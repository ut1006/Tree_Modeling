//decide_candidates.cpp
#include "include/branch.h"
#include "include/branches.h"
#include "include/branch.h"
#include "include/globals.h"
#include "include/draw.h"
#include <iostream>
#include <algorithm> 

std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>  decidedBranches;

//print green
void printgreen(){
std::cout << greenPercentage;
}

//try removing terminalBranches from branches. trial_index operates this. 
void trial_prune() {
    // Check if trial_index is within the bounds of terminalBranches
    if (trial_index >= terminalBranches.size()) {
        decided_num++;
        return;
    }

    // Convert terminalBranches to a vector for indexed access
    std::vector<std::pair<std::tuple<float, float, float>, std::tuple<float, float, float>>> terminalBranchVec(terminalBranches.begin(), terminalBranches.end());
    
    // Get the parent and child coordinates for the specified trial_index
    auto trial_branch = terminalBranchVec[trial_index];
    auto trial_parent = trial_branch.second;
    auto trial_child = trial_branch.first;

    // Remove the branch from branches
    branches.erase(std::remove_if(branches.begin(), branches.end(), [&](const Branch& branch) {
        return (std::make_tuple(branch.parent_x, branch.parent_y, branch.parent_z) == trial_parent &&
                std::make_tuple(branch.child_x, branch.child_y, branch.child_z) == trial_child);
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
}
//collect green% and calc value of obj function, then decide which to prune.  
void collect_results(){


}


