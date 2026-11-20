#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include "Board.h"
#include "Station.h"
#include "Vertex.h"
#include "Connection.h"
#include <stdexcept>
#include <boost//graph/adjacency_list.hpp>

using namespace state;

Board::Board ( )
{   
    
    std::ifstream file(RESOURCE_DIR "/pos.txt");

    if (!file.is_open()) {
        throw std::runtime_error(
            "Impossible d'ouvrir pos.txt"
        );
    }

    int nbStations;
    file >> nbStations;

    int id;
    float x, y;
    int count = 0;

    while (file >> id >> x >> y)
    {
        addStation(Station(id, {.x = x, .y = y}));
        count++;
    }

    std::cout << "Stations chargees : "
              << count << " / "
              << nbStations << std::endl;
/*
    addConnection(0, 1, Connection(Taxi));
    addConnection(1, 2, Connection(Bus));
    addConnection(0, 3, Connection(Bus));
    addConnection(1, 3, Connection(Taxi));
    addConnection(1, 4, Connection(Train));
    addConnection(2, 4, Connection(Bus));
    addConnection(3, 4, Connection(Taxi));
    addConnection(4, 5, Connection(Bus));*/
}

Board::~Board () = default;

std::vector<int> Board::getNeighbors (int stationId)
{
    auto it = stationVertices.find(stationId);

    if (it == stationVertices.end()) {
        throw std::out_of_range("Station does not exist");
    }

    Vertex vertex = it->second;

    std::vector<int> neighbors;

    auto range = boost::adjacent_vertices(vertex, transportMap);

    for (auto iter = range.first; iter != range.second; ++iter) {
        neighbors.push_back(transportMap[*iter].getId());
    }

    return neighbors;
}

void Board::addStation (Station station)
{
    const int id = station.getId();

    if (stationVertices.find(id) != stationVertices.end()) {
        throw std::invalid_argument("Station ID already exists");
    }

    Vertex vertex = boost::add_vertex(station, transportMap);

    stationVertices[id] = vertex;
}

void Board::addConnection (int station1Id, int station2Id, Connection connection)
{
    auto it1 = stationVertices.find(station1Id);
    auto it2 = stationVertices.find(station2Id);

    if (it1 == stationVertices.end() ||it2 == stationVertices.end()) {
        throw std::invalid_argument("Station does not exist");
        }

    boost::add_edge(it1->second, it2->second, connection, transportMap);
}

bool Board::areConnected (int station1Id, int station2Id)
{
    auto it1 = stationVertices.find(station1Id);
    auto it2 = stationVertices.find(station2Id);

    if (it1 == stationVertices.end() ||
        it2 == stationVertices.end()) {
        return false;
        }

    auto result = boost::edge(it1->second, it2->second, transportMap);

    return result.second;
}

const Station& Board::getStation (int stationId) const
{
    auto it = stationVertices.find(stationId);

    if (it == stationVertices.end())
    {
        throw std::invalid_argument("Station does not exist");
    }
    return transportMap[it->second];
}

Station& Board::getStation (int stationId)
{
    auto it = stationVertices.find(stationId);

    if (it == stationVertices.end())
    {
        throw std::invalid_argument("Station does not exist");
    }
    return transportMap[it->second];
}