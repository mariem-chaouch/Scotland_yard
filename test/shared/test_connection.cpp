#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SharedTest
#endif
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Connection.h"
#include "state.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestConnectionTaxi)
{
    Connection c(Transport::Taxi);
    BOOST_CHECK(c.getTransport() == Transport::Taxi);
}

BOOST_AUTO_TEST_CASE(TestConnectionAllTransports)
{
    Connection taxi(Transport::Taxi);
    Connection bus(Transport::Bus);
    Connection train(Transport::Train);
    Connection boat(Transport::Boat);

    BOOST_CHECK(taxi.getTransport() == Transport::Taxi);
    BOOST_CHECK(bus.getTransport() == Transport::Bus);
    BOOST_CHECK(train.getTransport() == Transport::Train);
    BOOST_CHECK(boat.getTransport() == Transport::Boat);
}

BOOST_AUTO_TEST_CASE(TestConnectionDistinct)
{
    Connection a(Transport::Taxi);
    Connection b(Transport::Bus);
    BOOST_CHECK(a.getTransport() != b.getTransport());
}

BOOST_AUTO_TEST_CASE(TestConnectionConst)
{
    const Connection c(Transport::Train);
    BOOST_CHECK(c.getTransport() == Transport::Train);   // getTransport est const
}