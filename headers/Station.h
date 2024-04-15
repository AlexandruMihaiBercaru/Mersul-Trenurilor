//
// Created by Alex on 15.04.2024.
//

#ifndef OOP_STATION_H
#define OOP_STATION_H

#include <iostream>

class Station {
private:
    std::string stationName;
    int         stationID; //<->a node in the graph
public:
    explicit Station(std::string name_ = "", int code_ = 0);

    [[nodiscard]] const std::string &getStationName() const;

    [[nodiscard]] int getStationID() const;

    friend std::ostream &operator<<(std::ostream &out, const Station &myStation);
};

#endif //OOP_STATION_H
