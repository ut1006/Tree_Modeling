//decide_candidates.cpp
#include "include/branch.h"
#include "include/branches.h"
#include "include/branch.h"
#include "include/globals.h"
#include "include/draw.h"
#include <iostream>
#include <algorithm> 

std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>  decidedBranches;
std::vector<float> results;

// Check if a branch is in decidedBranches
bool isBranchDecided(const Branch& branch) {
    std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
    std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
    return decidedBranches.find(child) != decidedBranches.end() && decidedBranches[child] == parent;
}

// Function to print the terminal branch vector
void printTerminalBranchVec(const std::vector<std::pair<std::tuple<float, float, float>, std::tuple<float, float, float>>>& vec) {
    // std::cout << "Terminal Branch Vector:" << std::endl;
    // for (size_t i = 0; i < vec.size(); ++i) {
    //     auto parent = vec[i].first;
    //     auto child = vec[i].second;
    //     std::cout << "Index " << i << ": Parent(" << std::get<0>(parent) << ", " << std::get<1>(parent) << ", " << std::get<2>(parent)
    //               << ") -> Child(" << std::get<0>(child) << ", " << std::get<1>(child) << ", " << std::get<2>(child) << ")" << std::endl;
    // }
}
//try removing terminalBranches from branches based on Greedy Algorithm
void trial_prune() {
    

    std::set<std::tuple<float, float, float>> terminalPoints = findTerminalBranchPoints(BranchMap, false, false);
    //init the branch
    branches=initialBranches; 

    // Check if trial_index is within the bounds of terminalBranches
    if (trial_index >= terminalBranchVec.size()) {
        decided_num++;
        trial_index = 0;
        std::cout << "end ";
        return;
    }
    std::cout << trial_index ;

    // Print the terminalBranchVec content
    if (trial_index == 0) {
        printTerminalBranchVec(terminalBranchVec);
    }

    // Get the parent and child coordinates for the specified trial_index
    auto trial_branch = terminalBranchVec[trial_index];
    auto trial_parent = trial_branch.second;
    auto trial_child = trial_branch.first;

    // Remove the branch from branches
    branches.erase(std::remove_if(branches.begin(), branches.end(), [&](const Branch& branch) {
        return (std::make_tuple(branch.parent_x, branch.parent_y, branch.parent_z) == trial_parent &&
                std::make_tuple(branch.child_x, branch.child_y, branch.child_z) == trial_child) ||
               isBranchDecided(branch);
    }), branches.end());
    
    // Also prune the branches from decidedBranches
    for (const auto& decided_branch : decidedBranches) {
        auto decided_parent = decided_branch.second;
        auto decided_child = decided_branch.first;

        branches.erase(std::remove_if(branches.begin(), branches.end(), [&](const Branch& branch) {
            return (std::make_tuple(branch.parent_x, branch.parent_y, branch.parent_z) == decided_parent &&
                    std::make_tuple(branch.child_x, branch.child_y, branch.child_z) == decided_child);
        }), branches.end());
    }
    BranchMap = createBranchMap(branches);
    trial_index++;

}

//calculate 3d variance of the tree. But variance(y) may be preferable.
float calculateScatter(const std::set<std::tuple<float, float, float>>& terminalPoints) {
    // Center of Points
    float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
    for (const auto& point : terminalPoints) {
        centerX += std::get<0>(point);
        centerY += std::get<1>(point);
        centerZ += std::get<2>(point);
    }
    int numPoints = terminalPoints.size();
    centerX /= numPoints;
    centerY /= numPoints;
    centerZ /= numPoints;

    // Calculate square-mean from cernter of points
    float sumOfSquaredDistances = 0.0f;
    for (const auto& point : terminalPoints) {
        float dx = std::get<0>(point) - centerX;
        float dy = std::get<1>(point) - centerY;
        float dz = std::get<2>(point) - centerZ;
        sumOfSquaredDistances += dx * dx + dy * dy + dz * dz;
    }

    // calculate scatter
    float scatter = sumOfSquaredDistances / numPoints;
    return scatter;
}

// Return score of the tree from sunlight and variance. 
float objective_function(float greenPercentage, std::set<std::tuple<float, float, float>> terminalPoints){

    float score = 0;
    float scatter = calculateScatter(terminalPoints);
    int numBranch = terminalPoints.size();
    if (scatter > initialScatter*0.6){
        score = greenPercentage;
    }
    return score;
}



//collect green% and calc value of obj function, then decide which to prune.  
void aggregate_results(){
   
    float score = objective_function(greenPercentage, terminalPoints);
    
    results.push_back(score);

    if(trial_index == 0 && results.size() != 0){
        results.pop_back();
        std::cout << "start deciding" << std::endl;

        // Find the index of the maximum value in results vector
        auto maxElementIter = std::max_element(results.begin(), results.end());
        int maxIndex = std::distance(results.begin(), maxElementIter);

        // for(auto result : results){std::cout << result <<std::endl;}

        std::cout << "max value: " << results[maxIndex] << " Index of the highest score: " << maxIndex << std::endl;
        std::cout << "num of results: "<<results.size() <<std::endl;
        // Retrieve the decided branch from terminalBranches using maxIndex
        auto decidedBranch = terminalBranchVec[maxIndex];
        
        // Insert the decided branch into decidedBranches
        decidedBranches[decidedBranch.first] = decidedBranch.second;

        terminalBranchVec.erase(terminalBranchVec.begin() + maxIndex);

        std::cout << "Decided Branch:" << std::endl;
        for (const auto& pair : decidedBranches) {
            std::cout << " Parent: (" << std::get<0>(pair.second) << ", " << std::get<1>(pair.second) << ", " << std::get<2>(pair.second) << ")" ;
            std::cout << "Child: (" << std::get<0>(pair.first) << ", " << std::get<1>(pair.first) << ", " << std::get<2>(pair.first) << ")"<< std::endl; 
        }
        results.clear();
        terminalBranches.clear();
    }
}