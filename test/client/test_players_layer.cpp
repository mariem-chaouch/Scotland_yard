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

BOOST_AUTO_TEST_CASE(TestPlayersLayerDrawsUnknownDetectiveColorAndHidesMrX)
{
  GameState gameState;
  BoardLayer boardLayer(gameState.getBoard());
  boardLayer.init();

  sf::RenderTexture target;
  BOOST_REQUIRE(target.create(640, 480));
  boardLayer.draw(target);
  const auto bounds = boardLayer.getMapBounds();

  const auto players = gameState.getPlayers();
  BOOST_REQUIRE_GE(players.size(), 2);
  players[0]->setCurrentStationId(5);
  players[1]->setCurrentStationId(1);
  players[1]->setColor("purple");

  PlayersLayer playersLayer(gameState, boardLayer);
  target.clear(sf::Color::Magenta);
  playersLayer.draw(target);
  target.display();
  const auto image = target.getTexture().copyToImage();

  const auto colorAtStation = [&](int id) {
    const auto position = gameState.getBoard().getStation(id).getPosition();
    return image.getPixel(
        static_cast<unsigned>(bounds.left + position.x * bounds.width),
        static_cast<unsigned>(bounds.top + position.y * bounds.height));
  };
  BOOST_CHECK(colorAtStation(1) == sf::Color::White);
  BOOST_CHECK(colorAtStation(5) == sf::Color::Magenta);
}
