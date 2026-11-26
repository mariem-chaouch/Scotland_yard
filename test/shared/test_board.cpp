#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <stdexcept>

#include "../../src/shared/state/Board.h"
#include "../../src/shared/state/Station.h"
#include "../../src/shared/state/Connection.h"

using namespace state;

BOOST_AUTO_TEST_CASE(TestBoardStations)
{
    Board board;
    const Board& constBoard = board;

    Station& station = board.getStation(1);
    BOOST_CHECK_EQUAL(station.getId(), 1);
    BOOST_CHECK_EQUAL(constBoard.getStation(1).getId(), 1);

    BOOST_CHECK_THROW(board.getStation(300), std::invalid_argument);
    BOOST_CHECK_THROW(constBoard.getStation(300), std::invalid_argument);
    BOOST_CHECK_THROW(board.addStation(station), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestBoardConnections)
{
    Board board;
    constexpr int missingStation = 300;

    BOOST_CHECK(board.areConnected(1, 9));
    BOOST_CHECK(board.areConnected(9, 1));

    BOOST_CHECK(!board.areConnected(1, missingStation));
    BOOST_CHECK(!board.areConnected(missingStation, 1));
    BOOST_CHECK_THROW(board.addConnection(1, missingStation, Connection(Train)), std::invalid_argument);
    BOOST_CHECK_THROW(board.addConnection(missingStation, 1, Connection(Train)), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestBoardNeighbors)
{
    Board board;

    const auto neighbors = board.getNeighbors(1);
    BOOST_CHECK(std::find(neighbors.begin(), neighbors.end(), 9) != neighbors.end());
    BOOST_CHECK_THROW(board.getNeighbors(300), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestConnectingAnIsolatedStation)
{
    Board board;
    constexpr int newStation = 300;

    board.addStation(Station(newStation, Point{0, 0}));
    BOOST_CHECK(board.getNeighbors(newStation).empty());
    BOOST_CHECK(!board.areConnected(newStation, 1));

    board.addConnection(newStation, 1, Connection(Train));
    BOOST_CHECK(board.areConnected(newStation, 1));
    const auto neighbors = board.getNeighbors(newStation);
    BOOST_REQUIRE_EQUAL(neighbors.size(), 1);
    BOOST_CHECK_EQUAL(neighbors.front(), 1);
}
