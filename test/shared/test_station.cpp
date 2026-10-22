#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SharedTest
#endif
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Station.h"

#include "state.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}
BOOST_AUTO_TEST_CASE(TestStationConstructor)
{
    Station s(5, Point{10, 20});

    BOOST_CHECK_EQUAL(s.getId(), 5);
    BOOST_CHECK_EQUAL(s.getPosition().x, 10);
    BOOST_CHECK_EQUAL(s.getPosition().y, 20);
    BOOST_CHECK(!s.isTaken());
}

BOOST_AUTO_TEST_CASE(TestStationSetTaken)
{
    Station s(1, Point{1, 2});

    s.setTaken(true);
    BOOST_CHECK(s.isTaken());

    s.setTaken(false);
    BOOST_CHECK(!s.isTaken());
}

BOOST_AUTO_TEST_CASE(TestStationDefaultConstructor)
{
    Station s;
    BOOST_CHECK(!s.isTaken());
}