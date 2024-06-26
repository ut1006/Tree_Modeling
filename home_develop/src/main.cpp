//main.cpp
#include "include/callbacks.h"
#include "include/draw.h"
#include "include/branches.h"
#include "include/globals.h"
#include "include/branch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip> 
std::vector<Branch> branches;
std::vector<Branch> initialBranches;
std::set<std::tuple<float, float, float>> branchPoints;
std::set<std::tuple<float, float, float>> terminalBranchPoints;
std::set<std::tuple<float, float, float>> terminalPoints;
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> parentChildMap;
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> BranchMap;
std::string filename;
float initialScatter;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_number>" << std::endl;
        return -1;
    }

    int file_number = std::stoi(argv[1]);
    filename = "tree" + std::to_string(file_number) + ".csv";

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Tree viewer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set the position of the first window
    glfwSetWindowPos(window, 100, 100);


        // Create the second window
    GLFWwindow* window2 = glfwCreateWindow(800, 600, "Top View", NULL, NULL);
    if (!window2) {
        std::cerr << "Failed to create second GLFW window" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Set the position of the second window
    glfwSetWindowPos(window2, 920, 100);
        
    // Make the window's context current
    glfwMakeContextCurrent(window2);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwDestroyWindow(window2);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwDestroyWindow(window2);
        glfwTerminate();
        return -1;
    }

    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetKeyCallback(window2, key_callback);

    // Load the initial tree data
    readCSV(filename, branches);

    initialBranches = branches;
    parentChildMap = createParentChildMap(branches);
    BranchMap = createBranchMap(branches);
    branchPoints = findBranchPoints(branches);
    terminalBranchPoints = findTerminalBranchPoints(BranchMap, true);
    initialScatter = calculateScatter(terminalPoints);
    // Find the branch points

    std::cout << "Branching Points:\n";
    for (const auto& point : branchPoints) {
        std::cout << std::fixed << std::setprecision(6) << std::get<0>(point) << ", " 
                << std::get<1>(point) << ", " << std::get<2>(point) << std::endl;
    }


    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set up viewport and projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    // Enable vsync
    glfwSwapInterval(1);


    // Variable to keep track of the last update time
    double lastUpdateTime = glfwGetTime();

// Main loop
while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2)) {

    // // Check if one second has passed
    // double currentTime = glfwGetTime();
    // if (currentTime - lastUpdateTime >= 1.0) {
    //     // Update the tree data
    //     updateTree(filename, branches);
    //     lastUpdateTime = currentTime;
    // }
    
    trial_prune();
   
    // Render scene from secondary view
    renderSceneFromSecondaryView(window2);

    // Render scene from main view and calc green%. 
    renderSceneFromMainView(window);

    //collect result here
    aggregate_results();

    // Poll for and process events
    glfwPollEvents();
    
    
}

// Cleanup and terminate GLFW
glfwDestroyWindow(window2);
glfwDestroyWindow(window);
glfwTerminate();

return 0;
}