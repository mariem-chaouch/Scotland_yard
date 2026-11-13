#include "Rules.h"
#include "state/MrX.h"
#include "state/Detective.h"
#include "state/GameState.h"
#include "state/Transport.h"

namespace engine {

static const int NB_STATIONS = 199;
static const int LAST_TURN = 24;

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

    bool isPlayerValid = (playerId >= 0 && playerId < (int)players.size());
    if (!isPlayerValid) {
        return false;
    }

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
    const state::Ticket tickets[] = { state::TaxiTicket, state::BusTicket,
        state::TrainTicket, state::BlackTicket };
    int from = game.getPlayers()[playerId]->getCurrentStationId();
    for (int station : game.getBoard().getNeighbors(from)){
        for (state::Ticket t : tickets) {
            if (isLegalMove(game, playerId, station, t)){
                moves.push_back({station, t});
            }
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

bool Rules::isLegalDoubleMove(state::GameState& game, int playerId,
        int firstDestination, state::Ticket firstTicket,
            int secondDestination, state::Ticket secondTicket) {
    std::vector<state::Player*> players = game.getPlayers();
    bool isPlayerValid = (playerId >= 0 && playerId < (int)players.size());
    if (!isPlayerValid) {
        return false;
    }

    bool mrXUsesTwoBlackTickets = (firstTicket == state::BlackTicket && secondTicket == state::BlackTicket);
    if ( mrXUsesTwoBlackTickets && mrx->getNbBlackTickets() < 2) {
        return false;
    }

    //MrX can't use double ticket during his last turn
    if (game.getTurn() >= LAST_TURN) {
        return false;
    }

    state::MrX* mrx = dynamic_cast<state::MrX*>(players[playerId]);
    bool playerIsDetective = (mrx == nullptr);
    if (playerIsDetective || !mrx->hasDoubleTicket()) {
        return false;
    }

    bool isFirstMoveLegal = isLegalMove(game, playerId, firstDestination, firstTicket);
    if (!isFirstMoveLegal) {
        return false;
    }

    int positionBeforeFirstMove = mrx->getCurrentStationId();
    mrx->setCurrentStationId(firstDestination);
    bool isSecondMoveLegal = isLegalMove(game, playerId, secondDestination, secondTicket);
    mrx->setCurrentStationId(positionBeforeFirstMove);
    return isSecondMoveLegal;
}

}









