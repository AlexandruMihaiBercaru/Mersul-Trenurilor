//
// Created by Alex on 15.04.2024.
//

#include "../headers/Routes.h"
#include <algorithm>

Routes::Routes(const std::vector<Train> &allTrains_) : allTrains(allTrains_) {}

void Routes::arrivals(std::string &stationName) {
    std::cout << "----ARRIVALS TO: " << stationName << "----\n";
    bool exists = false;
    for (auto &currentTrain: allTrains) {
        auto it = std::find_if(currentTrain.getStops().begin(), currentTrain.getStops().end(),
                               [&stationName](const Station &obj) { return obj.getStationName() == stationName; });
        if (it != currentTrain.getStops().end() && stationName != currentTrain.getStops()[0].getStationName()) //statia apare intre opririle trenului si nu este prima
        {
            exists = true;
            std::cout << currentTrain.getTrainID() << " coming from: ";
            auto index = std::distance(currentTrain.getStops().begin(), it);
            for (int i = 0; i < index - 1; i++)
                std::cout << currentTrain.getStops()[i].getStationName() << " - ";
            std::cout << currentTrain.getStops()[index - 1].getStationName() << "\n";
        }
    }
    if (!exists)
        std::cout << "Currently there are no arrivals!\n";
}

void Routes::departures(std::string &stationName) {
    std::cout << "\n----DEPARTURES FROM: " << stationName << "----\n";
    bool exists = false;
    for (auto &currentTrain: allTrains) {
        auto it = std::find_if(currentTrain.getStops().begin(), currentTrain.getStops().end(), [&stationName](const Station &obj) { return obj.getStationName() == stationName; });
        int stopsCount = int(currentTrain.getStops().size());
        if (it != currentTrain.getStops().end() && stationName != currentTrain.getStops()[stopsCount -1].getStationName()) //statia apare intre opririle trenului si nu este ultima
        {
            exists = true;
            std::cout << currentTrain.getTrainID() << " going to: ";
            int index = int(std::distance(currentTrain.getStops().begin(), it));
            for (int i = index + 1; i < stopsCount - 1; i++)
                std::cout << currentTrain.getStops()[i].getStationName() << " - ";
            std::cout << currentTrain.getStops()[stopsCount - 1].getStationName() << "\n";
        }
    }
    if (!exists) {
        std::cout << "Currently there are no departures!\n";
    }
}

const std::vector<Train> &Routes::getAllTrains() const {
    return allTrains;
}