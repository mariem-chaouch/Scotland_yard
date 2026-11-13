//
// Created by 33651 on 12/11/2026.
//

#include "MoveCommand.h"
#include "Engine.h"
#include "state/Player.h"

engine::MoveCommand::MoveCommand(int playerId, int destination, state::Ticket ticket)
    : destination(destination), ticket(ticket){
    this->playerId = playerId;
    this->commandType = CommandType::MOVE;
}

engine::MoveCommand::~MoveCommand() {
}

bool engine::MoveCommand::execute(Engine *engine) {
    state::GameState& gameState = engine->getState();

    bool isPlayerMoveLegal = engine->getRules().isLegalMove(gameState, playerId, destination, ticket);
    if (!isPlayerMoveLegal) {
        return false;
    }

    state::Player* player = gameState.getPlayers()[playerId];
    return player->makeMove(destination, ticket);
}


Json::Value engine::MoveCommand::serialize() const{
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    v["destination"] = destination;
    v["ticket"] = (int)ticket;
    return v;
}
