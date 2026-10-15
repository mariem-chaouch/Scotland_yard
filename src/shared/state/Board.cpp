#include <unordered_map>
#include <vector>
#include "Graph.h"
#include "Board.h"
#include "Station.h"
#include "Connection.h"
#include <stdexcept>
#include <boost//graph/adjacency_list.hpp>

std::vector<int> Board::getNeighbors (int stationID)
{
    auto it = stationVertices.find(stationId);

    if (it == stationVertices.end()) {
        throw std::out_of_range("Station does not exist");
    }

    Vertex vertex = it->second;

    std::vector<int> neighbors;

    auto range = boost::adjacent_vertices(vertex, graph);

    for (auto iter = range.first; iter != range.second; ++iter) {
        neighbors.push_back(graph[*iter].getId());
    }

    return neighbors;
}

void Board::addStation (Station station)
{
    const int id = station.getId();

    if (stationVertices.find(id) != stationVertices.end()) {
        throw std::invalid_argument("Station ID already exists");
    }

    Vertex vertex = boost::add_vertex(graph);

    graph[vertex] = station;

    stationVertices[id] = vertex;
}

void Board::addConnection (int station1Id, int station2Id, Connection connection)
{
    auto it1 = stationVertices.find(id1);
    auto it2 = stationVertices.find(id2);

    if (it1 == stationVertices.end() ||it2 == stationVertices.end()) {
        throw std::invalid_argument("Station does not exist");
        }

    boost::add_edge(it1->second, it2->second, connection, graph);
}
bool Board::areConnected (int station1Id, int station2Id)
{
    auto it1 = stationVertices.find(id1);
    auto it2 = stationVertices.find(id2);

    if (it1 == stationVertices.end() ||
        it2 == stationVertices.end()) {
        return false;
        }

    auto result = boost::edge(it1->second, it2->second, graph);

    return result.second;
}
Station& Board::getStation (int stationId)
{
    auto it = stationVertices.find(stationId);

    if (it == stationVertices.end())
    {
        throw std::invalid_argument("Station does not exist");
    }
    return graph[it->second];
}