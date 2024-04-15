//
// Created by Alex on 15.04.2024.
//

#include "../headers/Display.h"

void Display::printMenu() {
    std::cout << "\t\t\t\n\nChoose an option:\n\n";
    std::cout << "\t1. Print info about the rail network (the lines and the minimal distances on rail between stations)\n";
    std::cout << "\t2. Arrivals to a given station\n";
    std::cout << "\t3. Departures from a given station\n";
    std::cout << "\n\n\t [Press 'Exit' to leave.]\n";
}

void Display::printNetworkInfos(const std::vector<Railway> &railNetwork, RailNetwork &railMap) {
    for (const auto &elem: railNetwork)
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