#include "Rules.h"
#include "state/MrX.h"
#include "state/Detective.h"
#include "state/GameState.h"
#include "state/Transport.h"
#include <algorithm>

namespace engine {

static const int LAST_TURN = 24;

// Les cartes Départ : chaque joueur tire sa station au hasard parmi elles.
// À vérifier avec les cartes de votre boîte de jeu.
static const std::vector<int> START_STATIONS = {
    35, 45, 51, 71, 78, 104, 106, 127, 132, 146, 166, 170, 172,
    13, 26, 29, 34, 50, 53, 91, 94, 103, 112, 117, 123, 138, 141, 155, 174, 197, 198
};

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

    bool isPlayerValid = (playerId >= 0 && playerId < (int) players.size());
    if (!isPlayerValid)
        return false;

    state::Player* player = players[playerId];
    state::MrX* mrx = dynamic_cast<state::MrX*>(player);
    state::Detective* detective = dynamic_cast<state::Detective*>(player);

    // 1. Le joueur doit posséder ce ticket
    if (mrx) {
        // Mister X prend taxi/bus/métro dans la pioche commune ; seul le Black Ticket est compté
        if (ticket == state::BlackTicket && !mrx->hasBlackTicket())
            return false;
    } else if (detective) {
        if (!detective->hasTicket(ticket))
            return false;                               // pas ce ticket (et jamais de Black)
    } else {
        return false;
    }

    // 2. Une liaison du bon transport doit relier la station actuelle à la destination
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

    // 3. La destination ne doit pas être occupée par un détective
    //    (un détective peut aller sur la case de Mister X : c'est une capture)
    for (int i = 0; i < (int) players.size(); i++) {
        if (i == playerId) continue;
        if (dynamic_cast<state::Detective*>(players[i]) &&
            players[i]->getCurrentStationId() == destination)
            return false;
    }
    return true;
}

// Tous les coups possibles d'un joueur : liste de (destination, ticket)
std::vector<std::pair<int, state::Ticket>> Rules::getLegalMoves(state::GameState& game,
                                                               int playerId) {
    std::vector<std::pair<int, state::Ticket>> moves;
    std::vector<state::Player*> players = game.getPlayers();
    if (playerId < 0 || playerId >= (int) players.size())
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
    return turn == 3 || turn == 8 || turn == 13 || turn == 18 || turn == 24;
}

// La partie est-elle terminée ?
bool Rules::isGameOver(state::GameState& game) {
    std::vector<state::Player*> players = game.getPlayers();
    if (players.empty())
        return true;
    if (game.getTurn() == 0)
        return false;                                   // tour 0 : placement, pas encore commencé

    // Mister X est le joueur 0 : un détective sur sa case = capture
    int mrxStation = players[0]->getCurrentStationId();
    for (int i = 1; i < (int) players.size(); i++) {
        if (players[i]->getCurrentStationId() == mrxStation)
            return true;
    }

    // Le dernier tour est terminé
    if (game.getTurn() > LAST_TURN)
        return true;

    // Mister X ne peut plus bouger au moment de son tour
    bool mrxToPlay = (game.getCurrentPlayer() == players[0]);
    if (mrxToPlay && getLegalMoves(game, 0).empty())
        return true;

    // Plus aucun détective ne peut bouger
    for (int i = 1; i < (int) players.size(); i++) {
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
    bool isPlayerValid = (playerId >= 0 && playerId < (int) players.size());
    if (!isPlayerValid)
        return false;

    // Pas de Coup double au dernier tour
    if (game.getTurn() >= LAST_TURN)
        return false;

    state::MrX* mrx = dynamic_cast<state::MrX*>(players[playerId]);
    bool playerIsDetective = (mrx == nullptr);
    if (playerIsDetective || !mrx->hasDoubleTicket())
        return false;

    bool mrXUsesTwoBlackTickets = (firstTicket == state::BlackTicket &&
                                   secondTicket == state::BlackTicket);
    if (mrXUsesTwoBlackTickets && mrx->getNbBlackTickets() < 2)
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

    if (game.getTurn() != 0)
        return false;                                   // seulement au tour 0
    if (playerId < 0 || playerId >= (int) players.size())
        return false;                                   // joueur inexistant

    bool found = std::find(START_STATIONS.begin(), START_STATIONS.end(), station)
                 != START_STATIONS.end();
    if (!found)
        return false;                                   // pas une carte Départ

    for (int i = 0; i < (int) players.size(); i++) {
        if (i != playerId && players[i]->getCurrentStationId() == station)
            return false;                               // déjà prise
    }
    return true;
}

}