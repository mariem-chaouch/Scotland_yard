#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SharedTest
#endif
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Game.h"
#include "../../src/shared/state/MrX.h"
#include "../../src/shared/state/Detective.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestGameConstructor)
{
    Game g;
    BOOST_CHECK_EQUAL(g.getTurn(), 0);   // 0 = placement initial
}

BOOST_AUTO_TEST_CASE(TestGameBoard)
{
    Game g;
    const Board& b1 = g.getBoard();
    const Board& b2 = g.getBoard();
    BOOST_CHECK_EQUAL(&b1, &b2);   // même Board renvoyé par référence
}

BOOST_AUTO_TEST_CASE(TestGamePlayers)
{
    Game g;
    std::vector<Player*> players = g.getPlayers();

    BOOST_REQUIRE_EQUAL(players.size(), 5);
    BOOST_CHECK(dynamic_cast<MrX*>(players[0]) != nullptr);
    BOOST_CHECK_EQUAL(players[0]->getColor(), "white");
    for (int i = 1; i < 5; i++) {
        BOOST_CHECK(dynamic_cast<Detective*>(players[i]) != nullptr);
    }
    BOOST_CHECK_EQUAL(players[1]->getColor(), "blue");
    BOOST_CHECK_EQUAL(players[4]->getColor(), "yellow");
}

BOOST_AUTO_TEST_CASE(TestGameDestructor)
{
    // Le destructeur est exécuté en sortie de portée (delete des joueurs)
    {
        Game g;
    }
    Game* g = new Game();
    delete g;
    BOOST_CHECK(true);
}