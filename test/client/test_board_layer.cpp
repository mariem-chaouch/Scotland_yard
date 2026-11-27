#include <boost/test/unit_test.hpp>

#include <algorithm>

#include "../../src/client/render/BoardLayer.h"
#include "../../src/shared/state/Board.h"

using namespace render;
using namespace state;

BOOST_AUTO_TEST_CASE(TestBoardLayerConstruction)
{
  Board board;

  BoardLayer boardLayer(board);

  BOOST_CHECK(boardLayer.getMapBounds() == sf::FloatRect{});
}

BOOST_AUTO_TEST_CASE(TestBoardLayerDrawsSelectionUntilOutsideClick)
{
  Board board;
  BoardLayer boardLayer(board);
  boardLayer.init();

  sf::RenderTexture target;
  BOOST_REQUIRE(target.create(640, 480));

  const auto drawBoard = [&] {
    target.clear(sf::Color::Black);
    boardLayer.draw(target);
    target.display();
    return target.getTexture().copyToImage();
  };

  const auto withoutSelection = drawBoard();
  const auto bounds = boardLayer.getMapBounds();
  BOOST_REQUIRE_GT(bounds.width, 0.f);
  BOOST_REQUIRE_GT(bounds.height, 0.f);
  const auto station = board.getStation(100).getPosition();
  BOOST_REQUIRE(boardLayer.selectStationAt({
      bounds.left + station.x * bounds.width,
      bounds.top + station.y * bounds.height
  }));

  const auto withSelection = drawBoard();
  const auto pixelData = withoutSelection.getPixelsPtr();
  const auto byteCount = target.getSize().x * target.getSize().y * 4;
  BOOST_CHECK(!std::equal(pixelData, pixelData + byteCount, withSelection.getPixelsPtr()));

  BOOST_REQUIRE(!boardLayer.selectStationAt({bounds.left - 100.f, bounds.top - 100.f}));
  const auto afterOutsideClick = drawBoard();
  BOOST_CHECK(std::equal(pixelData, pixelData + byteCount, afterOutsideClick.getPixelsPtr()));
}

BOOST_AUTO_TEST_CASE(TestBoardLayerSkipsEmptyMapTexture)
{
  class BoardLayerWithEmptyMap : public BoardLayer
  {
  public:
    explicit BoardLayerWithEmptyMap(const Board& board) : BoardLayer(board)
    {
      textures.push_back(std::make_unique<sf::Texture>());
    }
  };

  Board board;
  BoardLayerWithEmptyMap boardLayer(board);

  sf::RenderTexture target;
  BOOST_REQUIRE(target.create(320, 240));
  target.clear(sf::Color::Magenta);
  boardLayer.draw(target);
  target.display();

  const auto image = target.getTexture().copyToImage();
  const auto pixels = image.getPixelsPtr();
  const auto byteCount = image.getSize().x * image.getSize().y * 4;
  sf::Image expected;
  expected.create(image.getSize().x, image.getSize().y, sf::Color::Magenta);
  BOOST_CHECK(std::equal(pixels, pixels + byteCount, expected.getPixelsPtr()));
}
