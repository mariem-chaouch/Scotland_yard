//
// Created by eya on 11/13/26.
//
#include <boost/test/unit_test.hpp>

#include "state.h"
#include "engine.h"

using namespace ::state;
using namespace ::engine;

// Place un joueur directement (sans StartCommand) et met à jour taken
static void place(Engine& engine, int player, int station) {
  Player* p = engine.getState().getPlayers()[player];
  int from = p->getCurrentStationId();
  p->setCurrentStationId(station);
  engine.updateTaken(from, station);
}

// Mister X en 50, détectives en 13, 26, 29, 34, puis tour 1
static void placePlayers(Engine& engine) {
  engine.init();
  place(engine, 0, 50);
  place(engine, 1, 13);
  place(engine, 2, 26);
  place(engine, 3, 29);
  place(engine, 4, 34);
  engine.getState().setTurn(1);
}

BOOST_AUTO_TEST_CASE(TestRevealTurns)
{
  Rules rules;
  BOOST_CHECK(rules.isRevealTurn(3));
  BOOST_CHECK(rules.isRevealTurn(8));
  BOOST_CHECK(rules.isRevealTurn(13));
  BOOST_CHECK(rules.isRevealTurn(18));
  BOOST_CHECK(rules.isRevealTurn(24));
  BOOST_CHECK(!rules.isRevealTurn(1));
  BOOST_CHECK(!rules.isRevealTurn(4));
}

BOOST_AUTO_TEST_CASE(TestInitStartsAtTurnZero)
{
  Engine engine;
  engine.init();
  BOOST_CHECK_EQUAL(engine.getState().getTurn(), 0);
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 0);
  BOOST_CHECK(!engine.getRules().isGameOver(engine.getState()));   // pas fini au tour 0
}

BOOST_AUTO_TEST_CASE(TestLegalMoveTaxi)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 77);
  GameState& game = engine.getState();

  BOOST_CHECK(engine.getRules().isLegalMove(game, 0, 78, TaxiTicket));   // 77-78 taxi
  BOOST_CHECK(!engine.getRules().isLegalMove(game, 0, 1, TaxiTicket));   // 77-1 pas reliées
}

BOOST_AUTO_TEST_CASE(TestStationTakenByDetective)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 77);
  place(engine, 1, 78);                                    // un détective sur 78
  GameState& game = engine.getState();

  BOOST_CHECK(game.getBoard().isStationTaken(78));
  BOOST_CHECK(!engine.getRules().isLegalMove(game, 0, 78, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestGameOverWhenCaught)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!engine.getRules().isGameOver(engine.getState()));
  place(engine, 1, 50);                                    // détective sur Mister X
  BOOST_CHECK(engine.getRules().isGameOver(engine.getState()));
}

BOOST_AUTO_TEST_CASE(TestMrXMoves)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 1);
  GameState& game = engine.getState();

  BOOST_CHECK(engine.getRules().isLegalMove(game, 0, 46, TrainTicket));  // 1-46 métro
  BOOST_CHECK(engine.getRules().isLegalMove(game, 0, 46, BlackTicket));  // Black = tout

  place(engine, 0, 194);
  BOOST_CHECK(engine.getRules().isLegalMove(game, 0, 157, BlackTicket)); // 194-157 bateau
  BOOST_CHECK(!engine.getRules().isLegalMove(game, 0, 157, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestDetectiveTrain)
{
  Engine engine;
  placePlayers(engine);                                    // détective 1 en 13
  BOOST_CHECK(engine.getRules().isLegalMove(engine.getState(), 1, 46, TrainTicket)); // 13-46 métro
}

BOOST_AUTO_TEST_CASE(TestUnknownPlayer)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!engine.getRules().isLegalMove(engine.getState(), 99, 49, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestLegalMovesList)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!engine.getRules().getLegalMoves(engine.getState(), 0).empty());  // Mister X peut bouger
  BOOST_CHECK(engine.getRules().getLegalMoves(engine.getState(), 99).empty());  // joueur inconnu
}

BOOST_AUTO_TEST_CASE(TestGameContinuesThenEnds)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!engine.getRules().isGameOver(engine.getState()));
  engine.getState().setTurn(25);                           // après le dernier tour
  BOOST_CHECK(engine.getRules().isGameOver(engine.getState()));
}

BOOST_AUTO_TEST_CASE(TestDoubleMove)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 1);
  GameState& game = engine.getState();

  BOOST_CHECK(engine.getRules().isLegalDoubleMove(game, 0, 8, TaxiTicket, 18, TaxiTicket));  // 1→8→18
  BOOST_CHECK(!engine.getRules().isLegalDoubleMove(game, 1, 14, TaxiTicket, 15, TaxiTicket)); // détective
  BOOST_CHECK_EQUAL(game.getPlayers()[0]->getCurrentStationId(), 1);   // la simulation a remis Mister X en 1
}

BOOST_AUTO_TEST_CASE(TestLegalStart)
{
  Engine engine;
  engine.init();
  GameState& game = engine.getState();

  BOOST_CHECK(engine.getRules().isLegalStart(game, 0, 50));      // carte Départ
  BOOST_CHECK(!engine.getRules().isLegalStart(game, 0, 1));      // pas une carte Départ
  place(engine, 0, 50);
  BOOST_CHECK(!engine.getRules().isLegalStart(game, 1, 50));     // déjà prise
  game.setTurn(1);
  BOOST_CHECK(!engine.getRules().isLegalStart(game, 1, 13));     // plus au tour 0
}
BOOST_AUTO_TEST_CASE(TestWinner)
{
  Engine engine;
  placePlayers(engine);                                  // Mr X en 50
  BOOST_CHECK(engine.getRules().getWinner(engine.getState()) == NONE);       // la partie continue
  BOOST_CHECK_EQUAL(engine.getRules().getCapturer(engine.getState()), -1);   // personne ne l'a pris

  place(engine, 2, 50);                                  // le détective 2 arrive sur Mr X
  BOOST_CHECK(engine.getRules().getWinner(engine.getState()) == DETECTIVES);
  BOOST_CHECK_EQUAL(engine.getRules().getCapturer(engine.getState()), 2);
}

BOOST_AUTO_TEST_CASE(TestMrXWinsAfterLastTurn)
{
  Engine engine;
  placePlayers(engine);
  engine.getState().setTurn(25);                         // les 24 tours sont passés
  BOOST_CHECK(engine.getRules().getWinner(engine.getState()) == MRX);
  BOOST_CHECK_EQUAL(engine.getRules().getCapturer(engine.getState()), -1);
}
/* vim: set sw=2 sts=2 et : */