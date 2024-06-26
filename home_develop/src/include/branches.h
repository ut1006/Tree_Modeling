#ifndef BRANCHES_H
#define BRANCHES_H

#include <vector>
#include "branch.h"
#include <GLFW/glfw3.h>

void readCSV(const std::string& filename, std::vector<Branch>& branches);
std::set<std::tuple<float, float, float>> findBranchPoints(const std::vector<Branch>& branches);
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createParentChildMap(const std::vector<Branch>& branches);
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createBranchMap(const std::vector<Branch>& branches);
std::set<std::tuple<float, float, float>> findTerminalBranchPoints(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& branchMap);
void calculateAndPrintGreenPercentage(GLFWwindow* window) ;
void printgreen();
void trial_prune();
void collect_results();
#endif
