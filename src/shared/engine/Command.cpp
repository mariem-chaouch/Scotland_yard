#include "Command.h"

namespace engine {

Command::Command() : commandType(MOVE), playerId(0) {}

Command::~Command() {}

CommandType Command::getCommandType() const {
    return commandType;
}

void Command::setCommandType(CommandType type) {
    commandType = type;
}

int Command::getPlayerId() const {
    return playerId;
}

void Command::setPlayerId(int playerId) {
    this->playerId = playerId;
}

}
