#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SharedTest
#endif
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Game.h"

#include "state.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestGameSingleton)
{
    Game* g1 = Game::getInstance();
    Game* g2 = Game::getInstance();

    BOOST_CHECK(g1 != nullptr);
    BOOST_CHECK_EQUAL(g1, g2);   // même adresse = une seule instance
}

BOOST_AUTO_TEST_CASE(TestGameBoard)
{
    Game* g = Game::getInstance();

    const Board& b1 = g->getBoard();
    const Board& b2 = g->getBoard();
    BOOST_CHECK_EQUAL(&b1, &b2);   // même Board renvoyé par référence
}

BOOST_AUTO_TEST_CASE(TestGameTurns)
{
    Game* g = Game::getInstance();
    BOOST_CHECK_EQUAL(g->getTurns().size(), 0);   // aucun tour au départ
}

BOOST_AUTO_TEST_CASE(TestGamePlayers)
{
    Game* g = Game::getInstance();
    std::vector<Player*> players = g->getPlayers();

    BOOST_REQUIRE_EQUAL(players.size(), 5);
    BOOST_CHECK_EQUAL(players[0]->getColor(), "white");
    BOOST_CHECK_EQUAL(players[4]->getColor(), "yellow");
}

BOOST_AUTO_TEST_CASE(TestGameDestroy)
{
    Game* g1 = Game::getInstance();
    Game::destroyInstance();
    Game* g2 = Game::getInstance();   // recréée
    BOOST_CHECK(g2 != nullptr);
    BOOST_CHECK_EQUAL(g2->getPlayers().size(), 5);
}