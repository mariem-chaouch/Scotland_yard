#include <boost/test/unit_test.hpp>
#include "../../src/client/render/Layer.h"

using namespace render;

class TestLayer : public Layer
{
public:
  bool initialized = false;

  void init() override
  {
    initialized = true;
  }

  void draw(sf::RenderTarget& target) override
  {
  }
};

BOOST_AUTO_TEST_CASE(TestLayerConstruction)
{
  TestLayer layer;

  BOOST_CHECK_EQUAL(layer.initialized, false);
}

BOOST_AUTO_TEST_CASE(TestLayerInitialization)
{
  TestLayer layer;

  layer.init();

  BOOST_CHECK_EQUAL(layer.initialized, true);
}