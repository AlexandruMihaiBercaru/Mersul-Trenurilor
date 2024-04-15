//
// Created by Alex on 15.04.2024.
//

#ifndef OOP_RAILNETWORK_H
#define OOP_RAILNETWORK_H

#include <iostream>
#include <vector>
#include "Railway.h"


class RailNetwork {
private:
    int totalStationCount;
    std::vector<Railway> allRailways;
    std::vector<Station> allStations;
    std::vector< std::vector<int> > railConnections, railDistances;
public:
    RailNetwork(int totalStationCount_, const std::vector<Railway> &allRailways_, const std::vector<Station> &allStations_, std::vector< std::vector<int> >&railConnections_, std::vector< std::vector<int> >&railDistances_);

    void addConnectionsAndDistances();

    void updateConnectionsCalculateDistances();

    friend std::ostream &operator<<(std::ostream &out, const RailNetwork &myMap);

    void printDistancesFromStation(const std::string &myStationName);
};


#endif //OOP_RAILNETWORK_H
