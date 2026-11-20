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

BOOST_AUTO_TEST_CASE(TestSceneRun)
{
    GameState gameState;

    Scene scene(gameState);
    scene.runScene();
}