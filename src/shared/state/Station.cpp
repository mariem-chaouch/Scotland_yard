//
// Created by 33651 on 15/10/2026.
//
#include "Station.h"

state::Station::Station(int id, Point position): id(id), position(position) {
    taken = false;
}

state::Station::Station()
    : id(0), position({.x = 0, .y = 0}), taken(false)
{
}

int state::Station::getId(){
    return id;
}

state::Point state::Station::getPosition(){
    return position;
}

bool state::Station::isTaken(){
    return taken;
}

void state::Station::setTaken(bool taken) {
    this->taken = taken;
}