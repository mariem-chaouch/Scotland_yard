#include <boost/test/unit_test.hpp>

#include "../../src/client/render/PlayersLayer.h"
#include "../../src/client/render/BoardLayer.h"
#include "../../src/shared/state/GameState.h"

using namespace render;
using namespace state;

BOOST_AUTO_TEST_CASE(TestPlayersLayerConstruction)
{
  GameState gameState;

  BoardLayer boardLayer(gameState.getBoard());

  PlayersLayer playersLayer(gameState, boardLayer);

  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(TestPlayersLayerInitialization)
{
  GameState gameState;

  BoardLayer boardLayer(gameState.getBoard());

  PlayersLayer playersLayer(gameState, boardLayer);

  BOOST_CHECK_NO_THROW(playersLayer.init());
}