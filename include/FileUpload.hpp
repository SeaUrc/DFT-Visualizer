#pragma once

#include <vector>
#include <Point.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cctype>

#include "nfd.hpp"

std::string openFileDialog() {
    NFD::Guard nfdGuard;

    // auto-freeing memory
    NFD::UniquePath outPath;

    // prepare filters for the dialog
    nfdfilteritem_t filterItem[1] = {{"Source code", "csv"}};

    // show the dialog
    nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1);
    if (result == NFD_OKAY) {}
    else if (result == NFD_CANCEL) {}
    else {
        std::cout << "Error: " << NFD::GetError() << std::endl;
    }

    return outPath.get();
}

std::vector<Point> extractPointsFromCSV(const char *csvFile) {
    std::vector<Point> points;
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file!" << std::endl;
        return points;
    }

    std::string line;
    bool isHeader = false;

    if (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        if (std::getline(lineStream, cell, ',')) {
            if (!std::isdigit(cell[0])) {
                isHeader = true;
            }
        }
    }

    if (isHeader) {
        if (!std::getline(file, line)) {
            std::cerr << "CSV file is empty after header!" << std::endl;
            return points;
        }
    } else {
        file.clear();
        file.seekg(0, std::ios::beg);
    }

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        float coords[2];
        int i = 0;
        while (std::getline(lineStream, cell, ',')) {
            coords[i++] = std::stof(cell);
        }
        points.emplace_back(coords[0], coords[1]);
    }

    file.close();

    return points;
}