#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Board.h"
#include "../../src/shared/state/Station.h"
#include "../../src/shared/state/Connection.h"

using namespace state;

BOOST_AUTO_TEST_CASE(TestBoardConstruction)
{
    Board board;

    // The constructor should load the stations and connections
    // from the resource files without throwing an exception.
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(TestBoardStations)
{
    Board board;

    // Check that a station can be retrieved by its ID.
    // Replace 1 with a station ID that exists in your pos.txt.
    Station& station = board.getStation(1);

    BOOST_CHECK_EQUAL(station.getId(), 1);
}

BOOST_AUTO_TEST_CASE(TestBoardConnections)
{
    Board board;

    // Replace these IDs with two stations connected in graph.txt.
    const int station1 = 1;
    const int station2 = 9;

    BOOST_CHECK(board.areConnected(station1, station2));
    BOOST_CHECK(board.areConnected(station2, station1));
}

BOOST_AUTO_TEST_CASE(TestBoardNeighbors)
{
    Board board;

    // Replace these IDs with a station and its known neighbor
    // from graph.txt.
    const int stationId = 1;
    const int neighborId = 9;

    std::vector<int> neighbors = board.getNeighbors(stationId);

    BOOST_CHECK(
        std::find(neighbors.begin(), neighbors.end(), neighborId)
        != neighbors.end()
    );
}