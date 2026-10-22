#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Move.h"
#include "../../src/shared/state/Turn.h"

using namespace state;

BOOST_AUTO_TEST_CASE(move_exposes_its_values)
{
  const int destination = 42;
  Player* player = nullptr;
  Move move(destination, TrainTicket, player);

  BOOST_CHECK_EQUAL(move.getDestination(), destination);
  BOOST_CHECK_EQUAL(move.getTicket(), TrainTicket);
  BOOST_CHECK_EQUAL(move.getPlayer(), player);
}

BOOST_AUTO_TEST_CASE(turn_starts_empty)
{
  Turn turn;

  BOOST_CHECK(turn.getMoves().empty());
}

BOOST_AUTO_TEST_CASE(turn_numbers_are_incremented)
{
  Turn first;
  Turn second;

  BOOST_CHECK_EQUAL(second.getNumber(), first.getNumber() + 1);
}

