#include <boost/test/unit_test.hpp>

#include "../../src/client/render/BoardLayer.h"
#include "../../src/shared/state/Board.h"

using namespace render;
using namespace state;

BOOST_AUTO_TEST_CASE(TestBoardLayerConstruction)
{
  Board board;

  BoardLayer boardLayer(board);

  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(TestBoardLayerSelectionOutsideStations)
{
  Board board;

  BoardLayer boardLayer(board);

  // Click far away from the board.
  sf::Vector2f position(100.f, 0.f);

  BOOST_CHECK_EQUAL(
      boardLayer.selectStationAt(position),
      false
  );
}