#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "headers/Station.h"
#include "headers/Railway.h"
#include "headers/RailNetwork.h"
#include "headers/Train.h"
#include "headers/Routes.h"
#include "headers/Display.h"
#include "csv.hpp"

//class Wagon{
//    -------TO DO---------
//
//};

//class Ticket{
//   ----------TO DO--------
//};

using namespace csv;

int main() {
    std::cout << "\n\t----BALKANIK TRANSRAIL----";

    std::vector<Railway> railways;
    std::vector<Station> stationList;
    std::vector<std::vector<int> > railConnectionsMatrix(200, std::vector<int>(200, 0)), minimalDistancesMatrix(200,std::vector<int>(200,0));
    std::vector<Train> trainList;
    int allStationCnt = 0;

    CSVFormat format;
    // Keeping variable-length rows
    format.variable_columns(VariableColumnPolicy::KEEP);
    CSVReader railwaysReader("inputs/railway_lines.csv", format);

    /// for each line of the railway_lines file, a new Railway object is created; furthermore,
    /// the vector containing all the stations in the network gets updated when a new station is found
    for (CSVRow& row: railwaysReader) { // Iterator for a line of the file
        std::vector<Station> stationsOnRailway;
        std::vector<int> distancesOnRailway;

        int railwayIDTemp = row[0].get<int>();
        int stationCount = row[1].get<int>();
        for(int index = 2; index < stationCount + 2; index++)
        {
            std::string currentStationName = row[index].get<>();
            bool stationExists = false;
            /// checking if the station name alraedy exists in the allStations array
            for (const auto &elem: stationList)
                if (elem.getStationName() == currentStationName)
                    stationExists = true;
            if (!stationExists) ///station has not been already added
            {
                allStationCnt++;
                int stationIDTemp = allStationCnt;
                Station stationTemp(currentStationName, stationIDTemp);
                stationList.push_back(stationTemp);
                stationsOnRailway.push_back(stationTemp);
            }
            else //the station already exists in allStations, therefore I need to get the Station object in order to add it to the stationsOnRailway vector
            {
                int stationIDTemp{0};
                for (auto &elem: stationList) //elem = o statie
                    if (elem.getStationName() == currentStationName)
                        stationIDTemp = elem.getStationID();
                Station stationTemp(currentStationName, stationIDTemp);
                stationsOnRailway.push_back(stationTemp);
            }
        }
        for(int index = 2 + stationCount; index < 2 * stationCount + 1; index++)
            distancesOnRailway.push_back(row[index].get<int>());
        Railway railwayTemp(stationsOnRailway, distancesOnRailway, railwayIDTemp, stationCount);
        railways.push_back(railwayTemp);
    }

    RailNetwork railwayMap(allStationCnt, railways, stationList, railConnectionsMatrix, minimalDistancesMatrix);
    railwayMap.addConnectionsAndDistances();
    railwayMap.updateConnectionsCalculateDistances();

    CSVReader trainsReader("inputs/trains.csv", format);
    for (CSVRow& row: trainsReader) {
        Railway currentRailway;
        std::vector<Station> stopsList;
        int trainTempID = row[0].get<int>();
        std::string trainTypeTemp = row[1].get<>();
        int railwayIDTemp = row[2].get<int>();
        auto it = std::find_if(railways.begin(), railways.end(), [railwayIDTemp](const Railway &railway) {return railway.getRailwayID() == railwayIDTemp;});
        if (it != railways.end())
            currentRailway = *it;
        for (int index = 3; index < int(row.size()); index++) {
            std::string currentStop = row[index].get<>();
            auto it2 = std::find_if(stationList.begin(), stationList.end(),[&currentStop](const Station &obj) { return obj.getStationName() == currentStop; });
            if (it2 != stationList.end())
                stopsList.push_back(*it2);
        }
        Train trainTemp(trainTempID, trainTypeTemp, currentRailway, stopsList);
        trainList.push_back(trainTemp);
    }

    Routes myTrainNetwork(trainList);

    while (true) {
        std::string keyboardInput;
        const std::string options[4] = {"Exit", "1", "2", "3"};
        Display::printMenu();
        getline(std::cin, keyboardInput);
        if (keyboardInput == options[0])
            break;
        else if (keyboardInput == options[1])
            Display::printNetworkInfos(railways, railwayMap);
        else if (keyboardInput == options[2])
            Display::printArrivals(myTrainNetwork);
        else if (keyboardInput == options[3])
            Display::printDepartures(myTrainNetwork);
        else {
            std::cout << "Wrong input... try again:" << std::endl;
        }
    }
    return 0;
}