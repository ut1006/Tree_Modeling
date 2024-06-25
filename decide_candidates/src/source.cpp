#include "include/draw.h"
#include "include/branches.h"
#include "include/globals.h"
#include "include/branch.h"
#include "include/globals.h"
#include <GL/glu.h>

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
    gluLookAt(3.0 * zoom + posX + 2, 6.0 * zoom, 3.0 * zoom + posZ + 2, posX, 0, posZ, 0.0, 1.0, 0.0);

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

    drawTerminalBranchPoints(terminalBranchPoints) ;

    drawTerminalPoints(terminalPoints);

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void updateTree(const std::string& filename, std::vector<Branch>& branches) {
    readCSV(filename, branches);
    branchPoints = findBranchPoints(branches);
    parentChildMap = createParentChildMap(branches);
    BranchMap = createBranchMap(branches);
    terminalBranchPoints = findTerminalBranchPoints(BranchMap);
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

    float greenPercentage = (float)greenPixels / totalPixels * 100.0f;
    printf("\r\033[KGreen Ratio: %.2f%%   ", greenPercentage); 
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