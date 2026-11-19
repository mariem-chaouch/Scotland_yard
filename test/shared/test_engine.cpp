#include <boost/test/unit_test.hpp>

#include <memory>
#include "state.h"
#include "engine.h"

using namespace ::state;
using namespace ::engine;

// Tour 0 : chaque joueur se place, à son tour, avec une StartCommand
static void placeAll(Engine& engine) {
  engine.init();
  const int starts[] = { 50, 13, 26, 29, 34 };      // Mister X, puis les 4 détectives
  for (int i = 0; i < 5; i++) {
    engine.addCommand(std::make_unique<StartCommand>(i, starts[i]));
    engine.update();
  }
}

static int stationOf(Engine& engine, int player) {
  return engine.getState().getPlayers()[player]->getCurrentStationId();
}

BOOST_AUTO_TEST_CASE(TestPlacementTurnZero)
{
  Engine engine;
  placeAll(engine);

  BOOST_CHECK_EQUAL(stationOf(engine, 0), 50);
  BOOST_CHECK_EQUAL(stationOf(engine, 1), 13);
  BOOST_CHECK_EQUAL(stationOf(engine, 4), 34);
  BOOST_CHECK_EQUAL(engine.getState().getTurn(), 1);          // placement fini → tour 1
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 0);       // Mister X commence
  BOOST_CHECK(!engine.getState().getBoard().isStationTaken(50));
  BOOST_CHECK(engine.getState().getBoard().isStationTaken(13)); 
  BOOST_CHECK_EQUAL(engine.getRecord().size(), 5u);           // 5 StartCommand notées
  BOOST_CHECK_EQUAL(engine.getRecord()[0]["turn"].asInt(), 0);
}

BOOST_AUTO_TEST_CASE(TestStartOnTakenStationRefused)
{
  Engine engine;
  engine.init();
  engine.addCommand(std::make_unique<StartCommand>(0, 50));
  engine.update();
  engine.addCommand(std::make_unique<StartCommand>(1, 50));   // déjà prise
  engine.update();

  BOOST_CHECK_EQUAL(stationOf(engine, 1), 0);                 // le détective n'est pas placé
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 1);       // c'est toujours à lui
}

BOOST_AUTO_TEST_CASE(TestWrongPlayerIgnored)
{
  Engine engine;
  placeAll(engine);
  engine.addCommand(std::make_unique<MoveCommand>(1, 14, TaxiTicket));  // c'est au tour de Mister X
  engine.update();

  BOOST_CHECK_EQUAL(stationOf(engine, 1), 13);                // le détective n'a pas bougé
  BOOST_CHECK_EQUAL(engine.getRecord().size(), 5u);           // rien de noté
}

BOOST_AUTO_TEST_CASE(TestIllegalMoveRefused)
{
  Engine engine;
  placeAll(engine);
  engine.addCommand(std::make_unique<MoveCommand>(0, 1, TaxiTicket));   // 50-1 : pas reliées
  engine.update();

  BOOST_CHECK_EQUAL(stationOf(engine, 0), 50);
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 0);       // toujours à Mister X
}

BOOST_AUTO_TEST_CASE(TestPassRefusedWhenCanMove)
{
  Engine engine;
  placeAll(engine);
  engine.addCommand(std::make_unique<PassCommand>(0));        // Mister X peut bouger
  engine.update();

  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 0);       // il ne peut pas passer
}

BOOST_AUTO_TEST_CASE(TestFullRound)
{
  Engine engine;
  placeAll(engine);
  engine.addCommand(std::make_unique<MoveCommand>(0, 49, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(1, 14, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(2, 27, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(3, 41, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(4, 47, TaxiTicket));
  engine.update();

  BOOST_CHECK_EQUAL(stationOf(engine, 0), 49);
  BOOST_CHECK_EQUAL(stationOf(engine, 4), 47);
  BOOST_CHECK_EQUAL(engine.getState().getTurn(), 2);          // tout le monde a joué
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 0);

  // taken suit les déplacements
  BOOST_CHECK(!engine.getState().getBoard().isStationTaken(50));
  BOOST_CHECK(engine.getState().getBoard().isStationTaken(49));

  // l'historique : 5 placements + 5 déplacements, avec le bon tour
  BOOST_CHECK_EQUAL(engine.getRecord().size(), 10u);
  BOOST_CHECK_EQUAL(engine.getRecord()[5]["turn"].asInt(), 1);
  BOOST_CHECK_EQUAL(engine.getRecord()[5]["destination"].asInt(), 49);
}

BOOST_AUTO_TEST_CASE(TestDoubleMoveTurnThreeToFive)
{
  Engine engine;
  placeAll(engine);
  engine.getState().setTurn(3);                               // tour d'apparition

  engine.addCommand(std::make_unique<DoubleMoveCommand>(0, 49, TaxiTicket, 66, TaxiTicket));
  engine.update();

  BOOST_CHECK_EQUAL(stationOf(engine, 0), 66);                // les deux déplacements sont joués
  BOOST_CHECK_EQUAL(engine.getState().getTurn(), 4);          // le Coup double compte 2 tours
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 1);       // au tour des détectives

  MrX* mrx = dynamic_cast<MrX*>(engine.getState().getPlayers()[0]);
  BOOST_REQUIRE(mrx != nullptr);
  BOOST_CHECK_EQUAL(mrx->getLastKnownStationId(), 49);        // révélé au 1er coup (tour 3)
  BOOST_CHECK(mrx->hasDoubleTicket());                        // il lui en reste 1 sur 2
  BOOST_CHECK_EQUAL(engine.getRecord()[5]["turn"].asInt(), 3);// noté au tour où il a commencé

  engine.addCommand(std::make_unique<MoveCommand>(1, 14, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(2, 27, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(3, 41, TaxiTicket));
  engine.addCommand(std::make_unique<MoveCommand>(4, 47, TaxiTicket));
  engine.update();

  BOOST_CHECK_EQUAL(engine.getState().getTurn(), 5);          // Mister X rejoue au tour 5
  BOOST_CHECK_EQUAL(engine.getCurrentPlayerIndex(), 0);
}
BOOST_AUTO_TEST_CASE(TestNoMoveAfterGameOver)
{
  Engine engine;
  placeAll(engine);
  engine.getState().setTurn(25);                              // après le dernier tour : partie finie

  engine.addCommand(std::make_unique<MoveCommand>(0, 49, TaxiTicket));
  engine.update();

  BOOST_CHECK_EQUAL(stationOf(engine, 0), 50);                // Mister X n'a pas bougé
  BOOST_CHECK_EQUAL(engine.getRecord().size(), 5u);           // seulement les 5 placements
}

/* vim: set sw=2 sts=2 et : */
