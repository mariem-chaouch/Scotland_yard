//
// Created by 33651 on 12/11/2026.
//
#include "PassCommand.h"
#include "Engine.h"
#include "state/Detective.h"

namespace {
    // Un détective ne peut pas utiliser le bateau (il faudrait un ticket noir)
    bool detectiveCanUse(const state::Detective* d, state::Transport t) {
        switch (t) {
            case state::Transport::Taxi:  return d->hasTicket(state::Ticket::TaxiTicket);
            case state::Transport::Bus:   return d->hasTicket(state::Ticket::BusTicket);
            case state::Transport::Train: return d->hasTicket(state::Ticket::TrainTicket);
            default:                      return false;
        }
    }
}

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
    bool currentPlayerIsDetective = (detective!=nullptr);
    if (!currentPlayerIsDetective) {
        return false;
    }

    const state::Board& board = gameState.getBoard();
    int from = detective->getCurrentStationId();

    for (int neighbor : board.getNeighbors(from)) {
        bool isStationTaken = false;
        for (state::Player * otherPlayer : players) {
            bool isOtherPlayerDetective = dynamic_cast<state::Detective*>(otherPlayer) != nullptr;
            bool isOtherPlayerANeighbour = otherPlayer->getCurrentStationId() == neighbor;

            if (otherPlayer != detective && isOtherPlayerDetective && isOtherPlayerANeighbour) {
                isStationTaken = true;
                break;
            }
        }
        if (isStationTaken) continue;


        for (state::Transport t : board.getTransportsBetween(from, neighbor)) {
            bool canTheDetectiveGoToNeighbour = detectiveCanUse(detective, t);
            if (canTheDetectiveGoToNeighbour) {
                return false;
            }
        }
    }

    return true;
}

Json::Value engine::PassCommand::serialize() const {
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    return v;
}