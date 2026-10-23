//
// Created by 33651 on 15/10/2026.
//
#include "Station.h"
#include <iostream>

state::Station::Station(int id, Point position): id(id), position(position) {
    std::cout << "--- Constructeur station ---" << std::endl;
    taken = false;
}

state::Station::Station()
    : id(0), position({.x = 0, .y = 0}), taken(false)
{
}

state::Station::~Station() = default;

int state::Station::getId() const{
    return id;
}

state::Point state::Station::getPosition() const{
    return position;
}

bool state::Station::isTaken() const{
    return taken;
}

void state::Station::setTaken(bool taken) {
    this->taken = taken;
}