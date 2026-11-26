#include "Rules.h"
#include "state/MrX.h"
#include "state/Detective.h"
#include "state/GameState.h"
#include "state/Transport.h"
#include <algorithm>
#include <array>
#include <random>
namespace engine{
    namespace {
        constexpr int FIRST_TURN = 0;
        constexpr int LAST_TURN = 24;
        constexpr int MRX_INDEX = 0;
        constexpr int NB_BLACK_FOR_DOUBLE_BLACK = 2;
        constexpr std::array<int, 5> REVEAL_TURNS = {3, 8, 13, 18, 24};
        constexpr std::array<int, 31> START_STATIONS = {
            35, 45, 51, 71, 78, 104, 106, 127, 132, 146, 166, 170, 172,
            13, 26, 29, 34, 50, 53, 91, 94, 103, 112, 117, 123, 138, 141,
            155, 174, 197, 198};
    }

// Un ticket permet-il d'emprunter ce transport ?
static bool ticketMatches(state::Ticket ticket, state::Transport transport) {
    switch (ticket) {
        case state::TaxiTicket:  return transport == state::Taxi;
        case state::BusTicket:   return transport == state::Bus;
        case state::TrainTicket: return transport == state::Train;
        case state::BlackTicket: return true;   // le Black Ticket permet tout, y compris le bateau
        default:                 return false;
    }
}


// Le coup « joueur playerId va en destination avec ce ticket » est-il permis ?
bool Rules::isLegalMove(state::GameState& game, int playerId,
                        int destination, state::Ticket ticket) {
    std::vector<state::Player*> players = game.getPlayers();

    bool isPlayerValid = (playerId >= MRX_INDEX && playerId < (int) players.size());
    if (!isPlayerValid)
        return false;

    if (game.getTurn() == FIRST_TURN) {
        return false;
    }

    state::Player* player = players[playerId];
    if (!player->hasTicket(ticket)) {
        return false;
    }

    int from = player->getCurrentStationId();
    std::vector<state::Transport> transports =
        game.getBoard().getTransportsBetween(from, destination);
    bool linkOk = false;
    for (state::Transport t : transports) {
        if (ticketMatches(ticket, t)) {
            linkOk = true;
            break;
        }
    }
    if (!linkOk)
        return false;

    bool destinationIsTaken = game.getBoard().isStationTaken(destination);
    if (destinationIsTaken) {
        return false;
    }

    for (int i = 1; i < (int) players.size(); i++) {
        if (players[i]->getCurrentStationId() == destination)
            return false;
    }
    return true;
}

// Tous les coups possibles d'un joueur : liste de (destination, ticket)
std::vector<std::pair<int, state::Ticket>> Rules::getLegalMoves(state::GameState& game,
                                                               int playerId) {
    std::vector<std::pair<int, state::Ticket>> moves;
    std::vector<state::Player*> players = game.getPlayers();
    if (playerId < MRX_INDEX || playerId >= (int) players.size())
        return moves;                                   // joueur inexistant : aucun coup

    int from = players[playerId]->getCurrentStationId();
    if (from <= 0)
        return moves;                                   // pas encore placé (tour 0)

    // Les voisins, sans doublons (deux stations peuvent être reliées par taxi ET bus)
    std::vector<int> neighbors = game.getBoard().getNeighbors(from);
    std::sort(neighbors.begin(), neighbors.end());
    neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());

    const state::Ticket tickets[] = { state::TaxiTicket, state::BusTicket,
                                      state::TrainTicket, state::BlackTicket };
    for (int station : neighbors) {
        for (state::Ticket t : tickets) {
            if (isLegalMove(game, playerId, station, t))
                moves.push_back({station, t});
        }
    }
    return moves;
}

// Mister X se montre-t-il à la fin de ce tour ?
bool Rules::isRevealTurn(int turn) {
    return std::find(REVEAL_TURNS.begin(), REVEAL_TURNS.end(), turn) != REVEAL_TURNS.end();
}

// La partie est-elle terminée ?
bool Rules::isGameOver(state::GameState& game) {
    std::vector<state::Player*> players = game.getPlayers();
    if (players.empty())
        return true;                                    // LCOV_EXCL_LINE : GameState a toujours des joueurs
    if (game.getTurn() == FIRST_TURN) {
        return false;                                   // tour 0 : placement, pas encore commencé
    }
    bool MrXIsCaptured = (getCapturer(game) != -1);
    if (MrXIsCaptured) {
        return true;
    }

    if (game.getTurn() > LAST_TURN) {
        return true;
    }

    bool mrxToPlay = (game.getCurrentPlayer() == players[MRX_INDEX]);
    bool mrxCantMove = getLegalMoves(game, 0).empty();
    if (mrxToPlay && mrxCantMove)
        return true;

    // Plus aucun détective ne peut bouger
    for (int i = MRX_INDEX+1; i < (int) players.size(); i++) {
        if (!getLegalMoves(game, i).empty())
            return false;
    }
    return true;
}

// Le Coup double de Mister X est-il permis ?
bool Rules::isLegalDoubleMove(state::GameState& game, int playerId,
                              int firstDestination, state::Ticket firstTicket,
                              int secondDestination, state::Ticket secondTicket) {
    std::vector<state::Player*> players = game.getPlayers();
    bool isPlayerValid = (playerId >= MRX_INDEX && playerId < (int) players.size());
    if (!isPlayerValid)
        return false;

    // Pas de Coup double au dernier tour
    if (game.getTurn() >= LAST_TURN)
        return false;

    auto* mrx = dynamic_cast<state::MrX*>(players[playerId]);
    bool playerIsDetective = (mrx == nullptr);
    if (playerIsDetective || !mrx->hasDoubleTicket())
        return false;

    bool mrXUsesTwoBlackTickets = (firstTicket == state::BlackTicket &&
                                   secondTicket == state::BlackTicket);
    if (mrXUsesTwoBlackTickets && mrx->getNbBlackTickets() < NB_BLACK_FOR_DOUBLE_BLACK)
        return false;

    if (!isLegalMove(game, playerId, firstDestination, firstTicket))
        return false;

    // 2e déplacement depuis la nouvelle station : on simule, puis on annule
    int positionBeforeFirstMove = mrx->getCurrentStationId();
    mrx->setCurrentStationId(firstDestination);
    bool isSecondMoveLegal = isLegalMove(game, playerId, secondDestination, secondTicket);
    mrx->setCurrentStationId(positionBeforeFirstMove);
    return isSecondMoveLegal;
}

// Le joueur peut-il commencer la partie sur cette station ?
bool Rules::isLegalStart(state::GameState& game, int playerId, int station) {
    std::vector<state::Player*> players = game.getPlayers();

    if (game.getTurn() != FIRST_TURN) {
        return false;
    }
    if (playerId < MRX_INDEX || playerId >= (int) players.size())
        return false;                                   // joueur inexistant

    bool found = std::find(START_STATIONS.begin(), START_STATIONS.end(), station)
                 != START_STATIONS.end();
    if (!found)
        return false;                                   // pas une carte Départ

    for (int i = MRX_INDEX; i < (int) players.size(); i++) {
        if (i != playerId && players[i]->getCurrentStationId() == station)
            return false;                               // déjà prise
    }
    return true;
}

    // Numéro du détective qui est sur la case de Mr X (1 à 4), ou -1
    int Rules::getCapturer(state::GameState& game)  {
    std::vector<state::Player*> players = game.getPlayers();
    if (players.empty())
        return -1;                                      // LCOV_EXCL_LINE : GameState a toujours des joueurs

    int mrxStation = players[MRX_INDEX]->getCurrentStationId();
    for (int i = 1; i < (int) players.size(); i++) {
        if (players[i]->getCurrentStationId() == mrxStation)
            return i;
    }
    return -1;
}

    // Qui a gagné ? NONE tant que la partie continue
    Winner Rules::getWinner(state::GameState& game) {
    if (!isGameOver(game))
        return NONE;                                   // la partie continue

    std::vector<state::Player*> players = game.getPlayers();
    if (players.empty())
        return NONE;                                    // LCOV_EXCL_LINE : GameState a toujours des joueurs

    if (getCapturer(game) != -1)
        return DETECTIVES;                             // Mr X est capturé

    if (game.getTurn() > LAST_TURN)
        return MRX;                                    // il a tenu jusqu'au bout

    if (game.getCurrentPlayer() == players[MRX_INDEX] && getLegalMoves(game, MRX_INDEX




    ).empty())
        return DETECTIVES;                             // Mr X est bloqué

    return MRX;                                        // tous les détectives sont bloqués
}
    // La liste des cartes Départ
    std::vector<int> Rules::getStartStations() {
        return std::vector<int>(START_STATIONS.begin(), START_STATIONS.end());
    }
    // On mélange le paquet, puis chaque joueur prend la carte suivante
    std::vector<int> Rules::drawStartStations(int nbPlayers, unsigned int seed) {
        std::vector<int> deck(START_STATIONS.begin(), START_STATIONS.end());
        std::mt19937 generator(seed);
        std::shuffle(deck.begin(), deck.end(), generator);
        deck.resize(nbPlayers);
        return deck;
    }

}
