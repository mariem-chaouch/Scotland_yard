#include <boost/test/unit_test.hpp>

#include "../../src/client/render/Scene.h"
#include "../../src/shared/state/GameState.h"

using namespace render;
using namespace state;

BOOST_AUTO_TEST_CASE(TestSceneConstruction)
{
  GameState gameState;

  BOOST_CHECK_NO_THROW(
      Scene scene(gameState)
  );
}

BOOST_AUTO_TEST_CASE(TestSceneDrawFrame)
{
  GameState gameState;
  Scene scene(gameState);

  BOOST_REQUIRE_NO_THROW(scene.initScene());
  BOOST_CHECK_NO_THROW(scene.drawScene());
}

BOOST_AUTO_TEST_CASE(TestSceneRunReturnsWhenWindowIsClosed)
{
  GameState gameState;
  Scene scene(gameState);
  scene.window.close();

  BOOST_CHECK_NO_THROW(scene.runScene());
}
