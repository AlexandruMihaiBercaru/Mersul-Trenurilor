//
// Created by Alex on 15.04.2024.
//

#include "../headers/Display.h"

void Display::printMenu() {
    std::cout << "\t\t\t\n\nChoose an option:\n\n";
    std::cout << "\t1. Print info about the rail network (the lines and the minimal distances on rail between stations)\n";
    std::cout << "\t2. Arrivals to a given station\n";
    std::cout << "\t3. Departures from a given station\n";
    std::cout << "\t4. Find a train route between two given stations\n";
    std::cout << "\n\n\t [Type 'Exit' to leave.]\n";
}

void Display::printNetworkInfos(RailNetwork &railMap) {
    for (const auto &elem: railMap.getAllRailways())
        operator<<(std::cout, elem);
    std::cout << "\nType the name of a starting station to find the rail distances:\n";
    std::string myStation;
    std::getline(std::cin, myStation);
    railMap.printDistancesFromStation(myStation);
}

void Display::printArrivals(Routes &myRoutes) {
    std::cout << "\nType the name of a station:\n";
    std::string myStation;
    std::getline(std::cin, myStation);
    myRoutes.arrivals(myStation);
    std::cout
            << "\nTo find out further details about a train, type its train ID, or type 'Back' to return to the main menu.\n";
    std::getline(std::cin, myStation);
    if (myStation == (std::string) "Back")
        return;
    else {
        int input = std::stoi(myStation);
        for (auto &crtTrain: myRoutes.getAllTrains())
            if (input == crtTrain.getTrainID())
                std::cout << crtTrain;
    }
}

void Display::printDepartures(Routes &myRoutes) {
    std::cout << "\nType the name of a station:\n";
    std::string myStation;
    std::getline(std::cin, myStation);
    myRoutes.departures(myStation);
    std::cout
            << "\nTo find out further details about a train, type its train ID, or type 'Back' to return to the main menu.\n";
    std::getline(std::cin, myStation);
    if (myStation == (std::string) "Back")
        return;
    else {
        int input = std::stoi(myStation);
        for (auto &crtTrain: myRoutes.getAllTrains())
            if (input == crtTrain.getTrainID())
                std::cout << crtTrain;
    }
}

void Display::initialMessage() {
    std::cout << "\n\t\tWelcome...";
}

void Display::printRoute(Routes &myRoutes, RailNetwork &railMap) {
    std::vector<int> myPath(200, 0);
    std::cout << "\nType the name of the starting station:\n";
    std::string myStartStation;
    std::getline(std::cin, myStartStation);
    std::string myStopStation;
    std::cout << "\nType the name of the stopping station:\n";
    std::getline(std::cin, myStopStation);
    int departureStationID{0}, arrivalStationID{0};
    for(auto &elem2 : railMap.getAllStations())
    {
        if(elem2.getStationName() == myStartStation)
            departureStationID = elem2.getStationID();
        if(elem2.getStationName() == myStopStation)
            arrivalStationID = elem2.getStationID();
    }
    int routeLength = myRoutes.getRailDistancesMatrix()[departureStationID][arrivalStationID];
    if(routeLength <  0x3F3F3F3F)
    {
        std::cout << "\t\t\t||\tROUTE FOUND!\n\t\t\t||\tLENGTH: " << routeLength << " km\n\t\t\t||\tTHE PATH:\n\t\t\t\t|-->" << myStartStation;
        myRoutes.reconstructPath(departureStationID, arrivalStationID, railMap, myPath);
        for(auto &elem: myPath)
        {
            std::string cityName;
            for(auto &elem2 : railMap.getAllStations()) {
                if (elem2.getStationID() == elem)
                {
                    cityName = elem2.getStationName();
                    std::cout << "\n\t\t\t\t|-->" << cityName ;
                }
            }
        }
    }
    else
    {
        std::cout << "\n\n\t\t\t\tNO ROUTE FOUND!\n";
    }
}
