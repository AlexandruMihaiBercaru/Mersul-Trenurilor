//
// Created by Alex on 15.04.2024.
//

#include "../headers/Station.h"

Station::Station(const std::string& name_, const int code_) : stationName(name_), stationID(code_) {}

const std::string &Station::getStationName() const {
    return stationName;
}

int Station::getStationID() const {
    return stationID;
}

std::ostream &operator<<(std::ostream &out, const Station &myStation) {
    out << myStation.stationName;
    return out;
}
