#ifndef BRANCHES_H
#define BRANCHES_H

#include <vector>
#include "branch.h"


void readCSV(const std::string& filename, std::vector<Branch>& branches);
std::set<std::tuple<float, float, float>> findBranchPoints(const std::vector<Branch>& branches);
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createParentChildMap(const std::vector<Branch>& branches);
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createBranchMap(const std::vector<Branch>& branches);
std::set<std::tuple<float, float, float>> findTerminalBranchPoints(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& branchMap);

#endif
