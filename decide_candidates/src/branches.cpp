#include "include/branches.h"
#include "include/globals.h"
#include "include/branch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip> 

void readCSV(const std::string& filename, std::vector<Branch>& branches) {
    std::ifstream file( "trees/" +filename);
    std::string line;

    // Skip the header
    std::getline(file, line);

    // Read branch data
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Branch branch;
        char comma;
        ss >> branch.parent_x >> comma >> branch.parent_y >> comma >> branch.parent_z >> comma
           >> branch.child_x >> comma >> branch.child_y >> comma >> branch.child_z >> comma
           >> branch.thickness;
        branches.push_back(branch);
    }
}

std::set<std::tuple<float, float, float>> findBranchPoints(const std::vector<Branch>& branches) {
    std::map<std::tuple<float, float, float>, int> parentCount; // Map to count occurrences of parent coordinates

    for (const auto& branch : branches) {
        // Skip branches with thickness == -1 (leaves)
        if (branch.thickness == -1) {
            continue;
        }

        // Increment count of parent coordinates
        std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
        parentCount[parent]++;

        // Check if parent coordinates occur more than once and thickness is not -1
        if (parentCount[parent] > 1) {
            branchPoints.insert(parent);
        }
    }

    return branchPoints;
}

std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createParentChildMap(const std::vector<Branch>& branches) {
  
    // You should consider initializing the variables before using in local! 
    std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> parentChildMap;
    // Without this line, new PC-map are added into existing map rather overwriting.   

    for (const auto& branch : branches) {
        std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
        std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
        parentChildMap[child] = parent;
    }

    return parentChildMap;
}

std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createBranchMap(const std::vector<Branch>& branches) {

    for (const auto& branch : branches) {
        if (branch.thickness != -1) {
            std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
            std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
            BranchMap[child] = parent;
        }
    }

    return BranchMap;
}

std::set<std::tuple<float, float, float>> findTerminalBranchPoints(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& BranchMap) {
    std::map<std::tuple<float, float, float>, int> pointCount;
    std::set<std::tuple<float, float, float>> terminalBranchPoints;

    // Count how many times coordinates emerge
    for (const auto& pair : BranchMap) {
        const auto& child = pair.first;
        const auto& parent = pair.second;
        pointCount[child]++;
        pointCount[parent]++;
    }

    // Search terminal points
    for (const auto& pair : pointCount) {
        if (pair.second == 1 && pair.first != std::make_tuple(0.0f, 0.0f, 0.0f)) {
            terminalPoints.insert(pair.first);
        }
    }

    for (const auto& point : terminalPoints) {
        std::cout << std::fixed << std::setprecision(6) << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << std::endl;
    }

    // Trace back from each terminal point to find terminal branching points
    for (const auto& terminal : terminalPoints) {
        auto current = terminal;
        std::cout << std::endl;
        while (true) {
            std::cout << ".";
            std::cout << "(" << std::get<0>(current) << ", " << std::get<1>(current) << ", " << std::get<2>(current) << ")";

            auto parentIt = BranchMap.find(current);
            if (parentIt == BranchMap.end()) {
                break; // Reached the root or no parent found
            }
            current = parentIt->second; // Trace back to the root
            std::cout << "(" << std::get<0>(current) << ", " << std::get<1>(current) << ", " << std::get<2>(current) << ")";

            if (branchPoints.find(current) != branchPoints.end()) {
                terminalBranchPoints.insert(current);
                std::cout << "1";
                break;
            }
        }
    }

    std::cout << "Terminal Branch Points:" << std::endl;
    for (const auto& point : terminalBranchPoints) {
        std::cout << "(" << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << ")" << std::endl;
    }

    return terminalBranchPoints;
}
