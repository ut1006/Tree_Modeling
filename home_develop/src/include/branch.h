#ifndef BRANCH_H
#define BRANCH_H

#include <vector>
#include <tuple>
#include <set>
#include <map>

struct Branch {
    float parent_x, parent_y, parent_z;
    float child_x, child_y, child_z;
    float thickness;
};

extern std::vector<Branch> branches;
extern std::set<std::tuple<float, float, float>> branchPoints;
extern std::set<std::tuple<float, float, float>> terminalBranchPoints;
extern std::set<std::tuple<float, float, float>> terminalPoints;
extern std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> BranchMap;
extern std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> terminalBranches;
extern std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> decidedBranches;
#endif
