//
// Created by Alex on 15.04.2024.
//

#include "../headers/RailNetwork.h"

RailNetwork::RailNetwork(int totalStationCount_, const std::vector<Railway> &allRailways_,
                         const std::vector<Station> &allStations_, std::vector<std::vector<int>> &railConnections_,
                         std::vector<std::vector<int>> &railDistances_) :
        totalStationCount(totalStationCount_), allRailways(allRailways_),  allStations(allStations_), railConnections(railConnections_), railDistances(railDistances_) { }

void RailNetwork::addConnectionsAndDistances() {
    for(auto & elem : allRailways) //for each railway of allRailways, repeat the sequence
    {
        for(int i = 0; i < elem.countStations - 1; i++)
        {
            std::string departureStationName = elem.stationsOnRailway[i].getStationName();
            std::string arrivalStationName = elem.stationsOnRailway[i + 1].getStationName();
            int departureStationID, arrivalStationID;
            for(auto &elem2 : allStations) //for each station in allStations, repeat the following sequence
            {
                if(elem2.getStationName() == departureStationName)
                    departureStationID = elem2.getStationID(); //the ID becomes the name of a node in the graph
                if(elem2.getStationName() == arrivalStationName)
                    arrivalStationID = elem2.getStationID(); //the ID becomes the name of a node in the graph
            }
            this->railDistances[departureStationID][arrivalStationID] = elem.distancesBetweenStations[i];
            this->railDistances[arrivalStationID][departureStationID] = elem.distancesBetweenStations[i];
            this->railConnections[departureStationID][arrivalStationID] = 1;
            this->railConnections[arrivalStationID][departureStationID] = 1;
        }
    }
    //completing the distance matrix by adding infinite
    for(int i = 1; i <= totalStationCount; i++)
    {
        for(int j = 1; j <= totalStationCount; j++)
            if (railDistances[i][j] == 0 && i != j)
                railDistances[i][j] = 0x3F3F3F3F; //make static value
    }
}

void RailNetwork::updateConnectionsCalculateDistances() //connections matrix now contains 1 if there is a road from i to j + distances matrix contains the shortest path between node i and j (if there is one)
{
    for(int k = 1; k <= totalStationCount; k++)
        for(int i = 1; i <= totalStationCount; i++)
            for(int j = 1; j <= totalStationCount; j++)
            {
                if(i != j && railConnections[i][j] == 0)
                    railConnections[i][j] = railConnections[i][k] * railConnections[k][j];
                if(railDistances[i][j] > railDistances[i][k] + railDistances[k][j])
                    railDistances[i][j] = railDistances[i][k] + railDistances[k][j];
            }
}

std::ostream &operator<<(std::ostream &out, const RailNetwork &myMap) {
    out<<"\n\t\t--------MINIMAL RAIL DISTANCES:--------\n";
    for(int i = 1; i <= myMap.totalStationCount; i++)
    {
        out <<"\t\tFROM: " << myMap.allStations[i - 1].getStationName() << " TO: \n\t\t";
        for(int j = 1; j <= myMap.totalStationCount; j++)
            if(myMap.railDistances[i][j] != 0 && myMap.railDistances[i][j] != 0x3F3F3F3F)
                out << " - " << myMap.allStations[j - 1].getStationName() << ": " << myMap.railDistances[i][j] << " km;\n\t\t";
        out << "\n";
    }
    return out;
}

void RailNetwork::printDistancesFromStation(const std::string &myStationName) {
    bool found = false;
    int myStationID{0};
    for(auto &elem: allStations) {
        if (elem.getStationName() == myStationName)
        {found = true; myStationID = elem.getStationID();}
    }
    if (!found)
        std::cout<<"\nThe station does not exist, try again...";
    else {
        std::cout << "\n\t----MINIMAL DISTANCES ON RAIL----\n\t    FROM " << myStationName << " TO: \n";
        for(int j = 1; j <= totalStationCount; j++)
            if(railDistances[myStationID][j] != 0 && railDistances[myStationID][j] != 0x3F3F3F3F)
                std::cout << "\t - " << allStations[j - 1].getStationName() << ": " << railDistances[myStationID][j] << " km;\n";
    }
}