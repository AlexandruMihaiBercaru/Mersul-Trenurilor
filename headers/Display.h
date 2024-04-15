//
// Created by Alex on 15.04.2024.
//

#ifndef OOP_DISPLAY_H
#define OOP_DISPLAY_H

#include <iostream>
#include "Routes.h"
#include "RailNetwork.h"
#include <vector>


class Display {
public:
    static void printMenu();
    static void printNetworkInfos(const std::vector<Railway> &railNetwork, RailNetwork &railMap);
    static void printArrivals(Routes &myRoutes);
    static void printDepartures(Routes &myRoutes);
};


#endif //OOP_DISPLAY_H
