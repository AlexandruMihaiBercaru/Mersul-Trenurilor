//
// Created by Alex on 15.04.2024.
//

#ifndef OOP_ROUTES_H
#define OOP_ROUTES_H

#include <iostream>
#include "Train.h"

class Routes {
    std::vector<Train> allTrains;
    //to add: trainConnectionsMatrix + distances by trains
public:
    explicit Routes(const std::vector<Train> &allTrains_);

    void arrivals(std::string &stationName);

    void departures(std::string &stationName);

    [[nodiscard]] const std::vector<Train> &getAllTrains() const;
};

#endif //OOP_ROUTES_H
