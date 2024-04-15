#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "headers/Station.h"
#include "headers/Railway.h"
#include "headers/RailNetwork.h"
#include "headers/Train.h"
#include "headers/Routes.h"
#include "headers/Display.h"

//class Vagon{
//    -------TO DO---------
//    std::string confort;
//    int numarLocuri;
//};

//class Bilet{
//   ----------TO DO--------
//};

int main() {
    std::cout << "\n\t----BALKANIK TRANSRAIL----";
    std::ifstream fin;
    std::vector<Railway> railways; //salvez obiecte de tip railway, le citesc din fisierul allRailways.in
    std::vector<Station> stationList; //toate statiile din harta retelei
    std::vector<std::vector<int> > railConnectionsMatrix(200, std::vector<int>(200, 0)), minimalDistancesMatrix(200,
                                                                                                                std::vector<int>(
                                                                                                                        200,
                                                                                                                        0));
    std::string file_line;
    std::vector<Train> trainList;
    int totalStationCnt = 0;


    fin.open("inputs/magistrale.in");
    if (!fin) {
        std::cerr << "Error in opening the file" << std::endl;
        return 1;
    }
    while (getline(fin, file_line)) {
        Railway tempRailway(file_line, totalStationCnt, stationList);
        railways.push_back(tempRailway);
    }
    fin.close();
    //am terminat cu fisierul allRailways.in. Acum, in vectorul 'railways' am magistralele

    RailNetwork railwayMap(totalStationCnt, railways, stationList, railConnectionsMatrix, minimalDistancesMatrix);
    railwayMap.addConnectionsAndDistances();
    railwayMap.updateConnectionsCalculateDistances();

    fin.open("inputs/trenuri.in");
    if (!fin) {
        std::cerr << "Error in opening the file" << std::endl;
        return 1;
    }
    while (getline(fin, file_line)) {
        std::string tempStop;
        Railway crtRailway;
        std::vector<Station> stopsList;
        std::vector<std::string> line = Railway::split(file_line, ", ");
        int trainIDTemp = std::stoi(line[0]);
        std::string trainTypeTemp = line[1];
        int railwayIDTemp = std::stoi(line[2]);
        auto it = std::find_if(railways.begin(), railways.end(), [railwayIDTemp](const Railway &railway) {
            return
                    railway.getRailwayID() == railwayIDTemp;
        });
        if (it != railways.end())
            crtRailway = *it;
        for (int i = 3; i < int(line.size()); i++) {
            tempStop = line[i];
            auto it2 = std::find_if(stationList.begin(), stationList.end(), [&tempStop](const Station &obj) {
                return
                        obj.getStationName() == tempStop;
            });
            if (it2 != stationList.end())
                stopsList.push_back(*it2);
        }
        Train trainTemp(trainIDTemp, trainTypeTemp, crtRailway, stopsList);
        trainList.push_back(trainTemp);
    }
    fin.close();
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