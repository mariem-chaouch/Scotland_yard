//
// Created by 33651 on 12/11/2026.
//
#include "PassCommand.h"
#include "Engine.h"
#include "state/Detective.h"

engine::PassCommand::PassCommand(int playerId) {
    this->playerId = playerId;
    this->commandType = CommandType::PASS;
}

engine::PassCommand::~PassCommand() {
}

bool engine::PassCommand::execute(Engine *engine) {
    state::GameState& gameState = engine->getState();
    std::vector<state::Player*> players = gameState.getPlayers();

    bool isPlayerIdNotValid = (playerId < 0 || playerId >= (int)players.size());
    if (isPlayerIdNotValid) {
        return false;
    }

    auto* detective = dynamic_cast<state::Detective*>(players[playerId]);
    bool currentPlayerIsDetective = (detective != nullptr);
    if (!currentPlayerIsDetective) {
        return false;
    }

    // On ne peut passer que s'il n'existe aucun coup légal
    bool hasNoLegalMove = engine->getRules().getLegalMoves(gameState, playerId).empty();
    return hasNoLegalMove;
}

Json::Value engine::PassCommand::serialize() const {
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    return v;
}