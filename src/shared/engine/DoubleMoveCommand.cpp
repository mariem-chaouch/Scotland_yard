//
// Created by 33651 on 12/11/2026.
//
#include "DoubleMoveCommand.h"
#include "Engine.h"
#include "state/MrX.h"

namespace {
    // Un coup est légal si les stations sont reliées
    // TODO: vérifier aussi la compatibilité transport/ticket (canTravel)
    bool isMoveLegal(const state::Board& board, int from, int to) {
        return board.areConnected(from, to);
    }
}

engine::DoubleMoveCommand::DoubleMoveCommand(int playerId, int firstDestination, state::Ticket firstTicket,
    int secondDestination, state::Ticket secondTicket)
        : firstDestination(firstDestination), firstTicket(firstTicket),
            secondDestination(secondDestination), secondTicket(secondTicket) {
    this->playerId = playerId;
    this->commandType = CommandType::DOUBLE_MOVE;
}

engine::DoubleMoveCommand::~DoubleMoveCommand() {
}

bool engine::DoubleMoveCommand::execute(Engine *engine) {
    state::GameState& gameState = engine->getState();
    std::vector<state::Player*> players = gameState.getPlayers();

    bool isPlayerIdNotValid = (playerId < 0 || playerId >= (int)players.size());
    if (isPlayerIdNotValid) {
        return false;
    }
    state::Player* currentPlayer = players[playerId];

    auto* mrX = dynamic_cast<state::MrX*>(currentPlayer);
    bool currentPlayerIsMrX = (mrX != nullptr);
    if (!currentPlayerIsMrX) {
        return false;
    }

    if (!mrX->hasDoubleTicket()) {
        return false;
    }

    // TODO: si les deux tickets sont noirs, il faut en avoir 2
    // (nécessite un getter du nombre de tickets noirs dans MrX)
    bool usesBlack = (firstTicket == state::Ticket::BlackTicket
                   || secondTicket == state::Ticket::BlackTicket);
    if (usesBlack && !mrX->hasBlackTicket()) {
        return false;
    }

    const state::Board& board = gameState.getBoard();
    if (!isMoveLegal(board, currentPlayer->getCurrentStationId(), firstDestination)) {
        return false;
    }
    if (!isMoveLegal(board, firstDestination, secondDestination)) {
        return false;
    }

    mrX->use_double_ticket();

    mrX->useTransportTicket(firstTicket);
    mrX->setCurrentStationId(firstDestination);

    mrX->useTransportTicket(secondTicket);
    mrX->setCurrentStationId(secondDestination);
    return true;
}

Json::Value engine::DoubleMoveCommand::serialize() const {
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    v["firstDestination"] = firstDestination;
    v["firstTicket"] = (int)firstTicket;
    v["secondDestination"] = secondDestination;
    v["secondTicket"] = (int)secondTicket;
    return v;
}