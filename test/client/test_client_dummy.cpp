#include <boost/test/unit_test.hpp>
#include <client/Example.h>

#include <SFML/Graphics.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestSFML)
{
  {
    ::sf::Texture texture;
    BOOST_CHECK(texture.getSize() == ::sf::Vector2<unsigned int> {});
  }
}

BOOST_AUTO_TEST_CASE(Main_client_compilation_test)
{
  {
    // Just check that the code compiles
    client::Example x;
    x.setY(42);
    BOOST_CHECK_EQUAL(x.y, 42);
  }
}

/* vim: set sw=2 sts=2 et : */
