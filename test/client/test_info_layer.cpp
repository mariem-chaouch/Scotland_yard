#include <boost/test/unit_test.hpp>

#include "../../src/client/render/InfoLayer.h"

using namespace render;

BOOST_AUTO_TEST_CASE(TestInfoLayerConstruction)
{
  InfoLayer layer;

  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(TestInfoLayerInitialization)
{
  InfoLayer layer;
  BOOST_CHECK_NO_THROW(layer.init());
}