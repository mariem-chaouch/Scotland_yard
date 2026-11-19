
#include "MoveCommand.h"
#include "Engine.h"
#include "state/Player.h"
#include "state/MrX.h"

namespace engine {

    MoveCommand::MoveCommand(int playerId, int destination, state::Ticket ticket)
        : destination(destination), ticket(ticket) {
        this->playerId = playerId;
        this->commandType = CommandType::MOVE;
    }

    MoveCommand::~MoveCommand() {}

    int MoveCommand::getDestination() const {
        return destination;
    }

    state::Ticket MoveCommand::getTicket() const {
        return ticket;
    }

    // Joue le déplacement s'il respecte les règles
    bool MoveCommand::execute(Engine* engine) {
        state::GameState& gameState = engine->getState();

        if (!engine->getRules().isLegalMove(gameState, playerId, destination, ticket))
            return false;                               // coup interdit : rien ne change

        state::Player* player = gameState.getPlayers()[playerId];
        int from = player->getCurrentStationId();       // station avant le déplacement
        bool hasMoved = player->makeMove(destination, ticket);
        if (!hasMoved)
            return false;

        engine->updateTaken(from, destination);         // occupation des stations

        // Mister X se montre s'il joue pendant un tour d'apparition
        auto* mrX = dynamic_cast<state::MrX*>(player);
        if (mrX != nullptr && engine->getRules().isRevealTurn(gameState.getTurn()))
            mrX->setLastKnownStationId(destination);

        return true;
    }

    Json::Value MoveCommand::serialize() const {
        Json::Value v;
        v["type"] = (int) commandType;
        v["player"] = playerId;
        v["destination"] = destination;
        v["ticket"] = (int) ticket;
        return v;
    }

}
