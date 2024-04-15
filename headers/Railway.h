//
// Created by Alex on 15.04.2024.
//

#ifndef OOP_RAILWAY_H
#define OOP_RAILWAY_H

#include <string>
#include <iostream>
#include <vector>
#include "Station.h"



//!  \brief Defines the railway objects.
/*!
  A railway(or rail line) refers to the physical element of the transport system.
*/
class Railway {
private:
    std::vector<Station> stationsOnRailway;
    std::vector<int> distancesBetweenStations;
    /// \brief the ID of the rail line
    int railwayID{0};
    int countStations{0};

public:
    Railway() = default;

    Railway(const std::vector<Station> &stationOnRailway_, const std::vector<int> &distancesBetweenStations_, int railwayID_, int countStations_);

    //Constructor mai specific?? - pentru citire allRailways din fisier + creare obiecte de tip statie (simultan)
    Railway(const std::string &oneLine, int &allStationCnt, std::vector<Station> &allStations);

    [[nodiscard]] int railwayLength() const;

    [[nodiscard]] int getRailwayID() const;

    static std::vector<std::string> split(const std::string& s,  const std::string& delimiter);

    friend std::ostream &operator<<(std::ostream &out, const Railway &myRailway);
    friend class RailNetwork;
};

#endif //OOP_RAILWAY_H
