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
    std::ifstream pos_file(std::string(RESOURCE_DIR) + "/pos.txt");

    if (!pos_file.is_open()) {
        throw std::runtime_error("Cannot open ../resources/pos.txt");
    }

    int station_nbr, id, x, y;
    pos_file >> station_nbr;
    for (int i = 0; i < station_nbr; i++) {
        pos_file >> id;
        pos_file >> x;
        pos_file >> y;
        addStation(Station(id, {.x = x, .y = y}));
        //std::cout << x<< "\t" << y << std::endl;
    }

    pos_file.close();

    int connection_nbr, station1, station2;
    std::string mean;
    Transport transport;

    std::ifstream graph_file(std::string(RESOURCE_DIR) + "/graph.txt");

    if (!graph_file.is_open()) {
        throw std::runtime_error("Cannot open ../resources/graph.txt");
    }

    graph_file >> station_nbr;
    graph_file >> connection_nbr;
    for (int i = 0; i < connection_nbr; i++) {
        graph_file >> station1;
        graph_file >> station2;
        graph_file >> mean;

        if (mean == "Taxi") {
            transport = Taxi;
        } else if (mean == "Bus") {
            transport = Bus;
        } else if (mean == "Train") {
            transport = Train;
        }else if (mean == "Boat") {
            transport = Boat;
        } else {
            std::cerr << "Unknown transport: " << mean << std::endl;
            continue;
        }

        addConnection(station1, station2, Connection(transport));
        std::cout << station1<< "\t" << station2 << "\t" << mean<< std::endl;
    }

    graph_file.close();
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

Station& Board::getStation (int stationId)
{
    auto it = stationVertices.find(stationId);

    if (it == stationVertices.end())
    {
        throw std::invalid_argument("Station does not exist");
    }
    return transportMap[it->second];
}