//
// Created by Alex on 16.04.2024.
//

#include "../headers/App.h"

void App::startApp() {
    std::vector<std::vector<int> > connections1 (200, std::vector<int>(200,0));
    std::vector<std::vector<int> > distances1 (200, std::vector<int>(200,0));
    std::vector<std::vector<int> > connections2 (200, std::vector<int>(200,0));
    std::vector<std::vector<int> > distances2 (200, std::vector<int>(200,0));
    std::vector<Station> myStations;
    std::vector<Railway> myRailways;
    std::vector<Train> myTrains;


    Display::initialMessage();
    Network *tempMap = new RailNetwork(connections1, distances1, myRailways, myStations);
    RailNetwork *myRailMap = dynamic_cast<RailNetwork*>(tempMap);

    if (myRailMap != nullptr)
    {
        myRailMap->parseRailsCSV();
        myRailMap->addConnectionsAndDistances();
        myRailMap->updateConnectionsCalculateDistances();
        Network *tempTrainMap = new Routes(connections2, distances2, myTrains);
        Routes *myTrainMap = dynamic_cast<Routes*>(tempTrainMap);
        if (myTrainMap != nullptr)
        {
            myTrainMap->parseTrainsCSV(*myRailMap);
            myTrainMap->buildTrainConnectionsDistances(*myRailMap);

            while (true) {
                std::string keyboardInput;
                const std::string options[5] = {"Exit", "1", "2", "3", "4"};
                Display::printMenu();
                getline(std::cin, keyboardInput);
                if (keyboardInput == options[0])
                    break;
                else if (keyboardInput == options[1])
                    Display::printNetworkInfos(*myRailMap);
                else if (keyboardInput == options[2])
                    Display::printArrivals(*myTrainMap);
                else if (keyboardInput == options[3])
                    Display::printDepartures(*myTrainMap);
                else if(keyboardInput == options[4])
                    Display::printRoute(*myTrainMap, *myRailMap);
                else {
                    std::cout << "Wrong input... try again:" << std::endl;
                }
            }
            delete myTrainMap;
        }
        else{
            std::cout << "Second Cast failed" << "\n";
        }
        delete myRailMap;
    }
    else{
        std::cout << "First Cast failed" << "\n";
    }
}
