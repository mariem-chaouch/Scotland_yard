#include <boost/test/unit_test.hpp>
#include <set>
#include <algorithm>
#include <vector>
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
  BOOST_CHECK(!Rules::isGameOver(engine.getState()));   // pas fini au tour 0
}

BOOST_AUTO_TEST_CASE(TestLegalMoveTaxi)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 77);
  GameState& game = engine.getState();

  BOOST_CHECK(Rules::isLegalMove(game, 0, 78, TaxiTicket));   // 77-78 taxi
  BOOST_CHECK(!Rules::isLegalMove(game, 0, 1, TaxiTicket));   // 77-1 pas reliées
}

BOOST_AUTO_TEST_CASE(TestStationTakenByDetective)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 77);
  place(engine, 1, 78);                                    // un détective sur 78
  GameState& game = engine.getState();

  BOOST_CHECK(game.getBoard().isStationTaken(78));
  BOOST_CHECK(!Rules::isLegalMove(game, 0, 78, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestGameOverWhenCaught)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!Rules::isGameOver(engine.getState()));
  place(engine, 1, 50);                                    // détective sur Mister X
  BOOST_CHECK(Rules::isGameOver(engine.getState()));
}

BOOST_AUTO_TEST_CASE(TestMrXMoves)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 1);
  GameState& game = engine.getState();

  BOOST_CHECK(Rules::isLegalMove(game, 0, 46, TrainTicket));  // 1-46 métro
  BOOST_CHECK(Rules::isLegalMove(game, 0, 46, BlackTicket));  // Black = tout

  place(engine, 0, 194);
  BOOST_CHECK(Rules::isLegalMove(game, 0, 157, BlackTicket)); // 194-157 bateau
  BOOST_CHECK(!Rules::isLegalMove(game, 0, 157, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestDetectiveTrain)
{
  Engine engine;
  placePlayers(engine);                                    // détective 1 en 13
  BOOST_CHECK(Rules::isLegalMove(engine.getState(), 1, 46, TrainTicket)); // 13-46 métro
}

BOOST_AUTO_TEST_CASE(TestUnknownPlayer)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!Rules::isLegalMove(engine.getState(), 99, 49, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestLegalMovesList)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!Rules::getLegalMoves(engine.getState(), 0).empty());  // Mister X peut bouger
  BOOST_CHECK(Rules::getLegalMoves(engine.getState(), 99).empty());  // joueur inconnu
}

BOOST_AUTO_TEST_CASE(TestGameContinuesThenEnds)
{
  Engine engine;
  placePlayers(engine);
  BOOST_CHECK(!Rules::isGameOver(engine.getState()));
  engine.getState().setTurn(25);                           // après le dernier tour
  BOOST_CHECK(Rules::isGameOver(engine.getState()));
}

BOOST_AUTO_TEST_CASE(TestDoubleMove)
{
  Engine engine;
  placePlayers(engine);
  place(engine, 0, 1);
  GameState& game = engine.getState();

  BOOST_CHECK(Rules::isLegalDoubleMove(game, 0, 8, TaxiTicket, 18, TaxiTicket));  // 1→8→18
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 1, 14, TaxiTicket, 15, TaxiTicket)); // détective
  BOOST_CHECK_EQUAL(game.getPlayers()[0]->getCurrentStationId(), 1);   // la simulation a remis Mister X en 1
}

BOOST_AUTO_TEST_CASE(TestLegalStart)
{
  Engine engine;
  engine.init();
  GameState& game = engine.getState();

  BOOST_CHECK(Rules::isLegalStart(game, 0, 50));      // carte Départ
  BOOST_CHECK(!Rules::isLegalStart(game, 0, 1));      // pas une carte Départ
  place(engine, 0, 50);
  BOOST_CHECK(!Rules::isLegalStart(game, 1, 50));     // déjà prise
  game.setTurn(1);
  BOOST_CHECK(!Rules::isLegalStart(game, 1, 13));     // plus au tour 0
}
BOOST_AUTO_TEST_CASE(TestWinner)
{
  Engine engine;
  placePlayers(engine);                                  // Mr X en 50
  BOOST_CHECK(Rules::getWinner(engine.getState()) == NONE);       // la partie continue
  BOOST_CHECK_EQUAL(Rules::getCapturer(engine.getState()), -1);   // personne ne l'a pris*

  place(engine, 2, 50);                                  // le détective 2 arrive sur Mr X
  BOOST_CHECK(Rules::getWinner(engine.getState()) == DETECTIVES);
  BOOST_CHECK_EQUAL(Rules::getCapturer(engine.getState()), 2);
}

BOOST_AUTO_TEST_CASE(TestMrXWinsAfterLastTurn)
{
  Engine engine;
  placePlayers(engine);
  engine.getState().setTurn(25);                         // les 24 tours sont passés
  BOOST_CHECK(Rules::getWinner(engine.getState()) == MRX);
  BOOST_CHECK_EQUAL(Rules::getCapturer(engine.getState()), -1);
}
/* vim: set sw=2 sts=2 et : */

// ---------- isLegalMove ----------
BOOST_AUTO_TEST_CASE(TestMoveRefusedAtTurnZero)
{
  Engine engine;
  engine.init();                                          // tour 0
  BOOST_CHECK(!Rules::isLegalMove(engine.getState(), 0, 49, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestMoveRefusedWithoutTicket)
{
  Engine engine;
  placePlayers(engine);
  GameState& game = engine.getState();
  Player* detective = game.getPlayers()[1];               // en 13, 13-14 taxi existe

  BOOST_CHECK(Rules::isLegalMove(game, 1, 14, TaxiTicket));
  for (int i = 0; i < 100 && detective->hasTicket(TaxiTicket); i++)
    detective->useTransportTicket(TaxiTicket);            // on vide ses tickets Taxi
  BOOST_REQUIRE(!detective->hasTicket(TaxiTicket));
  BOOST_CHECK(!Rules::isLegalMove(game, 1, 14, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestMoveRefusedOnPlayerNotMarkedTaken)
{
  Engine engine;
  placePlayers(engine);
  GameState& game = engine.getState();
  // On pose le détective 2 en 14 SANS passer par updateTaken
  game.getPlayers()[2]->setCurrentStationId(14);

  BOOST_REQUIRE(!game.getBoard().isStationTaken(14));
  BOOST_CHECK(!Rules::isLegalMove(game, 1, 14, TaxiTicket));  // 13-14 existe
}

// ---------- getLegalMoves ----------
BOOST_AUTO_TEST_CASE(TestLegalMovesUnplacedPlayer)
{
  Engine engine;
  engine.init();
  GameState& game = engine.getState();
  game.getPlayers()[0]->setCurrentStationId(0);
  BOOST_CHECK(Rules::getLegalMoves(game, 0).empty());
  BOOST_CHECK(Rules::getLegalMoves(game, -1).empty());
}

BOOST_AUTO_TEST_CASE(TestMrXBlockedDetectivesWin)
{
  Engine engine;
  placePlayers(engine);
  GameState& game = engine.getState();
  // Station 1 : voisins 8, 9, 46, 58 -> un détective sur chacun
  place(engine, 0, 1);
  place(engine, 1, 8);
  place(engine, 2, 9);
  place(engine, 3, 46);
  place(engine, 4, 58);

  BOOST_REQUIRE(game.getCurrentPlayer() == game.getPlayers()[0]);   // c'est à Mister X
  BOOST_CHECK(Rules::getLegalMoves(game, 0).empty());
  BOOST_CHECK(Rules::isGameOver(game));
  BOOST_CHECK(Rules::getWinner(game) == DETECTIVES);
}

BOOST_AUTO_TEST_CASE(TestDetectivesBlockedMrXWins)
{
  Engine engine;
  placePlayers(engine);
  GameState& game = engine.getState();
  for (int i = 1; i <= 4; i++)
    game.getPlayers()[i]->setCurrentStationId(0);         // "non placés" : aucun coup

  BOOST_REQUIRE(game.getCurrentPlayer() == game.getPlayers()[0]);
  BOOST_REQUIRE(!Rules::getLegalMoves(game, 0).empty());  // Mister X, lui, peut bouger
  BOOST_CHECK(Rules::isGameOver(game));
  BOOST_CHECK(Rules::getWinner(game) == MRX);
}

BOOST_AUTO_TEST_CASE(TestDoubleMoveRefusals)
{
  Engine engine;
  placePlayers(engine);                                   // Mister X en 50, tour 1
  GameState& game = engine.getState();

  BOOST_CHECK(Rules::isLegalDoubleMove(game, 0, 49, TaxiTicket, 66, TaxiTicket));
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 99, 49, TaxiTicket, 66, TaxiTicket));
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, -1, 49, TaxiTicket, 66, TaxiTicket));
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 0, 1, TaxiTicket, 66, TaxiTicket));   // 1er illégal
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 0, 49, TaxiTicket, 1, TaxiTicket));   // 2e illégal
  BOOST_CHECK_EQUAL(game.getPlayers()[0]->getCurrentStationId(), 50);

  game.setTurn(24);                                       // dernier tour
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 0, 49, TaxiTicket, 66, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestDoubleMoveNoDoubleTicketLeft)
{
  Engine engine;
  placePlayers(engine);
  GameState& game = engine.getState();
  MrX* mrx = dynamic_cast<MrX*>(game.getPlayers()[0]);
  BOOST_REQUIRE(mrx != nullptr);
  for (int i = 0; i < 10 && mrx->hasDoubleTicket(); i++)
    mrx->use_double_ticket();
  BOOST_REQUIRE(!mrx->hasDoubleTicket());
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 0, 49, TaxiTicket, 66, TaxiTicket));
}

BOOST_AUTO_TEST_CASE(TestDoubleMoveNotEnoughBlackTickets)
{
  Engine engine;
  placePlayers(engine);
  GameState& game = engine.getState();
  MrX* mrx = dynamic_cast<MrX*>(game.getPlayers()[0]);
  BOOST_REQUIRE(mrx != nullptr);
  for (int i = 0; i < 20 && mrx->getNbBlackTickets() >= 2; i++)
    mrx->useTransportTicket(BlackTicket);
  BOOST_REQUIRE(mrx->getNbBlackTickets() < 2);
  BOOST_CHECK(!Rules::isLegalDoubleMove(game, 0, 49, BlackTicket, 66, BlackTicket));
}

BOOST_AUTO_TEST_CASE(TestLegalStartUnknownPlayer)
{
  Engine engine;
  engine.init();
  BOOST_CHECK(!Rules::isLegalStart(engine.getState(), 99, 50));
  BOOST_CHECK(!Rules::isLegalStart(engine.getState(), -1, 50));
}

BOOST_AUTO_TEST_CASE(TestStartStationsDeck)
{
  std::vector<int> all = Rules::getStartStations();
  BOOST_CHECK_EQUAL(all.size(), 31u);

  std::vector<int> a = Rules::drawStartStations(5, 42);
  std::vector<int> b = Rules::drawStartStations(5, 42);
  BOOST_CHECK_EQUAL(a.size(), 5u);
  BOOST_CHECK(a == b);
  std::set<int> unique(a.begin(), a.end());
  BOOST_CHECK_EQUAL(unique.size(), 5u);
  for (int s : a)
    BOOST_CHECK(std::find(all.begin(), all.end(), s) != all.end());
}