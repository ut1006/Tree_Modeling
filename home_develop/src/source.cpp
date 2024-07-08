//source.cpp
#include "include/draw.h"
#include "include/branches.h"
#include "include/globals.h"
#include "include/branch.h"
#include "include/globals.h"
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

float greenPercentage;

#include <cmath>

void renderSceneFromMainView(GLFWwindow* window) {
    glfwMakeContextCurrent(window);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set up viewport and projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    // Render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Parameters for the orbiting camera
    static float angle = 0.0f;
    const float radius = 2.0f;
    const float cameraHeight = 5.5f;
    const float speed = 0.05f; // Rotation speed

    // Calculate camera position
    float camX = radius * cos(angle);
    float camZ = radius * sin(angle);
    float camY = cameraHeight;

    // Update the angle for the next frame
    angle += speed;

    // Make sure angle stays within 0 to 2*PI
    if (angle > 2 * M_PI) {
        angle -= 2 * M_PI;
    }

    // Set the camera to look at the origin (0, 0, 0)
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 3.0, 0.0);

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // Draw XYZ axes
    drawAxes();

    // Draw the ground plane
    drawGroundPlane();

    // Draw the tree
    drawTree(branches);

    // Draw branch points
    drawBranchPoints(branchPoints);

    drawTerminalBranchPoints(terminalBranchPoints);

    drawTerminalPoints(terminalPoints);

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void updateTree(const std::string& filename, std::vector<Branch>& branches) {
    std::vector<Branch> newBranches;
    readCSV(filename, newBranches);
    branches = std::move(newBranches);
}

void calculateAndPrintGreenPercentage(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    int totalPixels = width * height;
    int greenPixels = 0;

    glReadBuffer(GL_FRONT);

    GLubyte* pixels = new GLubyte[3 * totalPixels];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (int i = 0; i < totalPixels; i++) {
        if (pixels[i * 3] == 25 && pixels[i * 3 + 1] == 255 && pixels[i * 3 + 2] == 25) {
            greenPixels++;
        }
    }

    greenPercentage = (float)greenPixels / totalPixels * 100.0f;
    printf(" Green Ratio: %.2f%%   ", greenPercentage); 
    std::cout<< std::endl;
    fflush(stdout); 

    delete[] pixels;
}

void renderSceneFromSecondaryView(GLFWwindow* window) {
    glfwMakeContextCurrent(window);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set up viewport and projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    // Render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up camera at (5, 0, 0) looking at the origin (0, 0, 0)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);

    // Draw XYZ axes
    drawAxes();

    // Draw the ground plane
    drawGroundPlane();

    // Draw the tree
    drawTree(branches);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    calculateAndPrintGreenPercentage(window);
}