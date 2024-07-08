//branches.cpp
#include "include/branches.h"
#include "include/globals.h"
#include "include/branch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip> 

std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> terminalBranches;
std::vector<std::pair<std::tuple<float, float, float>, std::tuple<float, float, float>>> terminalBranchVec;


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

//Create map without leaves
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createBranchMap(const std::vector<Branch>& branches) {
    std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> BranchMap;
    for (const auto& branch : branches) {
        if (branch.thickness != -1) {
            std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
            std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
            BranchMap[child] = parent;
        }
    }

    return BranchMap;
}

std::set<std::tuple<float, float, float>> findTerminalBranchPoints(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& BranchMap, bool ifprint, bool ifupdate) {
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
    if(ifprint){
        std::cout << "terminalPoints:"<<std::endl;                     
        for (const auto& point : terminalPoints) {
            std::cout << std::fixed << std::setprecision(6) << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << std::endl;
        }
    }
    // Trace back from each terminal point to find terminal branching points
    for (const auto& terminal : terminalPoints) {
        auto current = terminal;
        std::tuple<float, float, float> previous = std::make_tuple(0.0f, 0.0f, 0.0f);

        while (true) {
            // if(ifprint){
            //     std::cout << ".";
            //     std::cout << "(" << std::get<0>(current) << ", " << std::get<1>(current) << ", " << std::get<2>(current) << ")";
            // }

            auto parentIt = BranchMap.find(current);
            if (parentIt == BranchMap.end()) {
                break; // Reached the root or no parent found
            }
            previous = current;
            current = parentIt->second; // Trace back to the root
            // if(ifprint){std::cout << "(" << std::get<0>(current) << ", " << std::get<1>(current) << ", " << std::get<2>(current) << ")";}
            if (branchPoints.find(current) != branchPoints.end()) {
                
                terminalBranchPoints.insert(current);
                terminalBranches[previous] = current;
                
                // if(ifprint){std::cout << "1" << std::endl;}
                break;
            }
        }
    }

    if(ifupdate){
    // Convert terminalBranches to a vector for indexed access
    terminalBranchVec.assign(terminalBranches.begin(), terminalBranches.end());
    }
    
    // if(trial_index==0){
    //     std::cout << "Terminal Branch Points:" << std::endl;
    //     for (const auto& point : terminalBranchPoints) {
    //         std::cout << "(" << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << ")" << std::endl;
    //     }

    // std::cout << "Terminal Branch Pairs:" << std::endl;
    // for (const auto& pair : terminalBranches) {
    //     std::cout << " Parent: (" << std::get<0>(pair.second) << ", " << std::get<1>(pair.second) << ", " << std::get<2>(pair.second) << ")" ;
    //     std::cout << "Child: (" << std::get<0>(pair.first) << ", " << std::get<1>(pair.first) << ", " << std::get<2>(pair.first) << ")"<< std::endl;  
    // }
    // }
    return terminalBranchPoints;
}
