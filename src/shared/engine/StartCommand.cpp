#include "StartCommand.h"
#include "Engine.h"
#include "state/Player.h"
#include "state/Detective.h"

engine::StartCommand::StartCommand(int playerId, int station)
    : station(station) {
    this->playerId = playerId;
    this->commandType = CommandType::START;
}

engine::StartCommand::~StartCommand() {
}

bool engine::StartCommand::execute(Engine* engine) {
    state::GameState& game = engine->getState();

    bool isStartLegal = engine->getRules().isLegalStart(game, playerId, station);
    if (!isStartLegal) {
        return false;
    }

    state::Player* player = game.getPlayers()[playerId];
    player->setCurrentStationId(station);

    bool isDetective = (dynamic_cast<state::Detective*>(player) != nullptr);
    if (isDetective) {
        game.setStationTaken(station, true);
    }
    return true;
}

Json::Value engine::StartCommand::serialize() const {
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    v["station"] = station;
    return v;
}
