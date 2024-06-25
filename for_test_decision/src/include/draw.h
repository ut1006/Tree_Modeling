#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include <tuple>
#include <set>
#include "branch.h"
#include "globals.h"
void drawAxes();
void drawGroundPlane();
void drawTree(const std::vector<Branch>& branches);
void drawBranchPoints(const std::set<std::tuple<float, float, float>>& branchPoints);
void drawTerminalBranchPoints(const std::set<std::tuple<float, float, float>>& terminalBranchPoints);
void drawTerminalPoints(const std::set<std::tuple<float, float, float>>& terminalPoints);
void renderSceneFromMainView(GLFWwindow* window) ;
void updateTree(const std::string& filename, std::vector<Branch>& branches);
void renderSceneFromSecondaryView(GLFWwindow* window);

#endif
