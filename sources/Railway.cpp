//
// Created by Alex on 15.04.2024.
//

#include "../headers/Railway.h"

Railway::Railway(const std::vector<Station> &stationOnRailway_, const std::vector<int> &distancesBetweenStations_,
                 int railwayID_, int countStations_)
        : stationsOnRailway(stationOnRailway_), distancesBetweenStations(distancesBetweenStations_),
          railwayID(railwayID_), countStations(countStations_) {}

Railway::Railway(const std::string &oneLine, int &allStationCnt, std::vector<Station> &allStations) {
    std::vector<std::string> line = split(oneLine, ", "), stationNamesTemp;
    std::string currentStationName;
    std::vector<int> distancesTemp;
    /// \brief An array of Station objects which memorises the stations built on the current railway
    std::vector<Station> stationsOnThisRailway;
    int stationIDTemp;
    this->railwayID = std::stoi(line[0]);
    this->countStations = std::stoi(line[1]);
    for (int i = 0; i < countStations; i++) //procesez pe rand statiile din fiecare railway, le salvez intr-un vector
    { //creez vectorul de statii din railway si il actualizez pe cel cu toate statiile
        currentStationName = line[2 + i];
        stationNamesTemp.push_back(currentStationName);
        /// \brief
        bool stationExists = false;
        for (const auto &elem: allStations)
            if (elem.getStationName() == currentStationName)
                stationExists = true;
        if (!stationExists) ///station has not been already added
        {
            allStationCnt++;
            stationIDTemp = allStationCnt;
            Station stationTemp(currentStationName, stationIDTemp);
            allStations.push_back(stationTemp);
            stationsOnThisRailway.push_back(stationTemp);
        } //am adaugat in vectorul cu toate statiile
        else //exista in vectorul cu toate statiile, o adaug in vectorul cu statiile pentru railway -- ii vreau codul? (caut in vector)
        {
            for (auto &elem: allStations) //elem = o statie
                if (elem.getStationName() == currentStationName)
                    stationIDTemp = elem.getStationID();
            Station stationTemp(currentStationName, stationIDTemp);
            stationsOnThisRailway.push_back(stationTemp);
        }
    }
    this->stationsOnRailway = stationsOnThisRailway;
    distancesTemp.reserve(countStations - 1);
    for (int i = 0; i < countStations - 1; i++)
        distancesTemp.push_back(std::stoi(line[2 + countStations + i]));
    this->distancesBetweenStations = distancesTemp;
}

int Railway::railwayLength() const {
    int totalLength = 0;
    for (int i: distancesBetweenStations)
        totalLength += i;
    return totalLength;
}

int Railway::getRailwayID() const {
    return railwayID;
}

std::ostream &operator<<(std::ostream &out, const Railway &myRailway) {
    out << "\nRAILWAY " << myRailway.railwayID << ": \nLENGTH: " << myRailway.railwayLength() << " km; \nSTATIONS: ";
    for (int i = 0; i < myRailway.countStations - 1; i++)
        out << myRailway.stationsOnRailway[i].getStationName() << " - ";
    out << myRailway.stationsOnRailway[myRailway.countStations - 1].getStationName() << "\n";
    return out;
}

std::vector<std::string> Railway::split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}