#include "DoubleMoveCommand.h"
#include "Engine.h"
#include "Rules.h"
#include "state/MrX.h"

// Les deux MoveCommand sont créées à partir des paramètres
engine::DoubleMoveCommand::DoubleMoveCommand(int playerId, int firstDestination, state::Ticket firstTicket,
    int secondDestination, state::Ticket secondTicket)
        : firstMoveCommand(playerId, firstDestination, firstTicket),
          secondMoveCommand(playerId, secondDestination, secondTicket) {
    this->playerId = playerId;
    this->commandType = CommandType::DOUBLE_MOVE;
}

engine::DoubleMoveCommand::~DoubleMoveCommand() {
}

bool engine::DoubleMoveCommand::execute(Engine* engine) {
    state::GameState& game = engine->getState();
    Rules& rules = engine->getRules();

    // Les deux coups sont vérifiés AVANT d'en jouer un seul
    bool isPlayerDoubleMoveLegal = rules.isLegalDoubleMove(game, playerId,
        firstMoveCommand.getDestination(), firstMoveCommand.getTicket(),
        secondMoveCommand.getDestination(), secondMoveCommand.getTicket());
    if (!isPlayerDoubleMoveLegal) {
        return false;
    }

    state::MrX* mrx = dynamic_cast<state::MrX*>(game.getPlayers()[playerId]);

    // 1er déplacement au tour courant
    // (la révélation éventuelle de Mister X est faite par MoveCommand)
    int turn = game.getTurn();
    if (!firstMoveCommand.execute(engine)) {
        return false;
    }

    // Le Coup double compte pour deux tours : le 2e déplacement a lieu au tour suivant
    game.setTurn(turn + 1);
    if (!secondMoveCommand.execute(engine)) {
        return false;
    }

    mrx->use_double_ticket();
    return true;
}

Json::Value engine::DoubleMoveCommand::serialize() const {
    Json::Value v;
    v["type"] = (int)commandType;
    v["player"] = playerId;
    v["first"] = firstMoveCommand.serialize();
    v["second"] = secondMoveCommand.serialize();
    return v;
}
