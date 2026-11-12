//
// Created by 33651 on 12/11/2026.
//

#include "MoveCommand.h"
#include "Engine.h"
#include "state/Player.h"
#include "state/Detective.h"
#include "state/MrX.h"

engine::MoveCommand::MoveCommand(int playerId, int destination, state::Ticket ticket)
    : destination(destination), ticket(ticket){
    this->playerId = playerId;
    this->commandType = CommandType::MOVE;
}

engine::MoveCommand::~MoveCommand() {
}

bool engine::MoveCommand::execute(Engine *engine) {
    state::GameState& gameState = engine->getState();
    std::vector<state::Player*> players = gameState.getPlayers();

    bool isPlayerNotValid = (playerId < 0 || playerId >= (int)players.size());
    if (isPlayerNotValid) {
        return false;
    }
    state::Player* currentPlayer = players[playerId];

    auto* detective = dynamic_cast<state::Detective*>(currentPlayer);
    auto* mrX = dynamic_cast<state::MrX*>(currentPlayer);
    bool currentPlayerIsDetective = (detective!=nullptr);
    bool currentPlayerIsMrX = (mrX!=nullptr);

    if (currentPlayerIsDetective) {
        if (!detective->hasTicket(ticket)) {
            return false;
        }
    } else if (currentPlayerIsMrX) {
        if (ticket == state::Ticket::BlackTicket && !mrX->hasBlackTicket()) {
            return false;
        }
    } else {
        return false;
    }

    if (!gameState.getBoard().areConnected(currentPlayer->getCurrentStationId(), destination)) {
        return false;
    }

    if (currentPlayerIsDetective) {
        for (state::Player * otherPlayer : players) {
            bool isOtherPlayerDetective = dynamic_cast<state::Detective*>(otherPlayer) != nullptr;
            bool isOtherPlayerInDestination = otherPlayer->getCurrentStationId() == destination;
            if (otherPlayer != currentPlayer && isOtherPlayerDetective && isOtherPlayerInDestination) {
                return false;
                }
        }
    }

    currentPlayer->useTransportTicket(ticket);
    currentPlayer->setCurrentStationId(destination);
    return true;
}


Json::Value engine::MoveCommand::serialize() const{
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    v["destination"] = destination;
    v["ticket"] = (int)ticket;
    return v;
}
