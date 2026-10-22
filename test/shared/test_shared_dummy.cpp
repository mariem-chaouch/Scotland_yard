#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SharedTest
#endif
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include "state.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

/* vim: set sw=2 sts=2 et : */