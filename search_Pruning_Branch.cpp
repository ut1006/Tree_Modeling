#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility> 
#include <chrono>
#include <map>
#include <set>
#include <random> 
#include <algorithm>
#include <iomanip> 
#include <unordered_set>
#include <unordered_map>

//g++ -o desktop_gen search_Pruning_Branch.cpp -lglfw -lGL -lGLU -lGLEW

// Structure to hold the branch information
struct Branch {
    float parent_x, parent_y, parent_z;
    float child_x, child_y, child_z;
    float thickness;
};

std::vector<Branch> branches;
// Global variable for branch points
std::set<std::tuple<float, float, float>> branchPoints;
std::set<std::tuple<float, float, float>> terminalBranchPoints;
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> parentChildMap;
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> BranchMap;
std::set<std::tuple<float, float, float>> terminalPoints;

// Function to find the branch points
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



// Function to create a parent-child map
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createParentChildMap(const std::vector<Branch>& branches) {

    for (const auto& branch : branches) {
        std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
        std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
        parentChildMap[child] = parent;
    }

    return parentChildMap;
}

// Function to create a branch map
std::map<std::tuple<float, float, float>, std::tuple<float, float, float>> createBranchMap(const std::vector<Branch>& branches) {

    for (const auto& branch : branches) {
        if (branch.thickness != -1) {
            std::tuple<float, float, float> parent(branch.parent_x, branch.parent_y, branch.parent_z);
            std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
            BranchMap[child] = parent;
        }
    }

    return BranchMap;
}

std::set<std::tuple<float, float, float>> findTerminalBranchPoints(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& BranchMap) {
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

    for (const auto& point : terminalPoints) {
        std::cout << std::fixed << std::setprecision(6) << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << std::endl;
    }

    // Trace back from each terminal point to find terminal branching points
    for (const auto& terminal : terminalPoints) {
        auto current = terminal;
        std::cout << std::endl;
        while (true) {
            std::cout << ".";
            std::cout << "(" << std::get<0>(current) << ", " << std::get<1>(current) << ", " << std::get<2>(current) << ")";

            auto parentIt = BranchMap.find(current);
            if (parentIt == BranchMap.end()) {
                break; // Reached the root or no parent found
            }
            current = parentIt->second; // Trace back to the root
            std::cout << "(" << std::get<0>(current) << ", " << std::get<1>(current) << ", " << std::get<2>(current) << ")";

            if (branchPoints.find(current) != branchPoints.end()) {
                terminalBranchPoints.insert(current);
                std::cout << "1";
                break;
            }
        }
    }

    std::cout << "Terminal Branch Points:" << std::endl;
    for (const auto& point : terminalBranchPoints) {
        std::cout << "(" << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << ")" << std::endl;
    }

    return terminalBranchPoints;
}



bool canTraceBackToRoot(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& parentChildMap, const std::tuple<float, float, float>& current, const std::tuple<float, float, float>& root) {
    auto it = current;
    while (it != root) {
        auto parentIt = parentChildMap.find(it);
        if (parentIt == parentChildMap.end()) {
            return false;
        }
        it = parentIt->second;
    }
    return true;
}


// Function to read the CSV file
void readCSV(const std::string& filename, std::vector<Branch>& branches) {
    std::ifstream file(filename);
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

// Global variables for OpenGL
float rotationX = 0.0f;
float rotationY = 0.0f;
float zoom = 1.0f;
float posX = 0.0f;
float posY = 0.0f;
float posZ = 0.0f;
bool dragging = false;
double lastMouseX = 0.0, lastMouseY = 0.0;
bool drawLeaves = true; 



// Mouse callback to update rotation angles
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool ctrlPressed = false; // Track Ctrl key state
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || 
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        ctrlPressed = true;
    } else {
        ctrlPressed = false;
    }

    if (dragging) {
        float xoffset = xpos - lastMouseX;
        float yoffset = ypos - lastMouseY;

        lastMouseX = xpos;
        lastMouseY = ypos;

        const float sensitivity = 0.01f; // Adjust the sensitivity here

        if (!ctrlPressed) {
            // Calculate the difference in mouse position
            float xdiff = xoffset * sensitivity;
            float ydiff = -yoffset * sensitivity;

            // Get the current modelview matrix
            GLfloat modelview[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

            // Calculate the camera direction vector
            float camX = modelview[2];
            float camY = modelview[6];
            float camZ = modelview[10];

            // Normalize the camera direction vector
            float camLength = sqrt(camX * camX + camY * camY + camZ * camZ);
            camX /= camLength;
            camY /= camLength;
            camZ /= camLength;

            // Calculate the movement direction in camera space
            float movX = camX * xdiff - camY * ydiff;
            float movY = camY * xdiff + camX * ydiff;
            float movZ = camZ * xdiff;

            // Update the position of the cylinder
            posX -= movX;
            posY += movY;
            posZ += movY; // Invert y-axis for consistency with other controls
        } else {
            // Rotational movement
            rotationX += yoffset * sensitivity * 10.f;
            rotationY += xoffset * sensitivity * 10.f;
        }
    }
}

// Mouse button callback to handle dragging state
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            dragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            dragging = false;
        }
    }
}

// Scroll callback to handle zooming
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom -= yoffset * 0.1f;
    if (zoom < 0.1f) zoom = 0.1f; // Prevent zooming too far out
}

// Key callback to handle exit on 'q' press and linear movement
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        drawLeaves = !drawLeaves;  // Toggle the drawLeaves variable
    }

    const float moveSpeed = 0.1f;

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        posZ -= moveSpeed;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        posZ += moveSpeed;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        posX -= moveSpeed;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        posX += moveSpeed;
    }
}

void drawAxes() {
    // X axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINES);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glEnd();

    // Y axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glBegin(GL_LINES);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glEnd();

    // Z axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void drawGroundPlane() {
    glColor3f(0.5f, 0.5f, 0.5f); // Set the ground plane color to gray
    glBegin(GL_QUADS);

    // Draw a large ground plane
    glVertex3f(-50.0f, -1.0f, -50.0f);
    glVertex3f(50.0f, -1.0f, -50.0f);
    glVertex3f(50.0f, -1.0f, 50.0f);
    glVertex3f(-50.0f, -1.0f, 50.0f);

    glEnd();
}

void drawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    // Draw the cylinder
    glPushMatrix();
    glTranslatef(x1, y1, z1);
    // Calculate the direction vector
    float dirX = x2 - x1;
    float dirY = y2 - y1;
    float dirZ = z2 - z1;

    // Calculate the length of the cylinder
    float length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

    // Normalize the direction vector
    float dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    dirX /= dirLength;
    dirY /= dirLength;
    dirZ /= dirLength;

    // Calculate the angle and axis for rotation
    float angleX = atan2(dirY, dirZ) * 180.0 / M_PI;
    float angleY = atan2(dirX, sqrt(dirY * dirY + dirZ *dirZ)) * 180.0 / M_PI;
    glRotatef(-angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);


    // Set the color to brown
    glColor3f(0.4f, 0.1f, 0.1f);



    gluCylinder(quad, radius, radius, length, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawEllipse(float majorAxis, float minorAxis) {
    const int numSegments = 36; // Number of segments to approximate ellipse
    const float angleIncrement = 2.0f * M_PI / numSegments; // Angle increment for each segment
    glBegin(GL_TRIANGLE_FAN);
    

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = majorAxis * cos(angle);
        float y = minorAxis * sin(angle);
        glVertex3f(x, y, 0.0f); // Draw a vertex at the calculated point
    }
    glEnd();
}

// Function to find the thickness of the parent branch
std::pair<bool, float> findParentThickness(const std::vector<Branch>& branches, float parent_x, float parent_y, float parent_z) {
    for (const auto& branch : branches) {
        if (branch.child_x == parent_x && branch.child_y == parent_y && branch.child_z == parent_z) {
            return {true, branch.thickness};
        }
    }
    return {false, 0.0f}; // Return false if the parent thickness is not found
}


void drawLeaf(const Branch& branch, const std::vector<Branch>& branches) {
    glPushMatrix();

    // Calculate direction
    float dirX = branch.child_x - branch.parent_x;
    float dirY = branch.child_y - branch.parent_y;
    float dirZ = branch.child_z - branch.parent_z;
    float length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

    // Normalize direction
    dirX /= length;
    dirY /= length;
    dirZ /= length;

    // Calculate angles
    float angleX = atan2(dirY, dirZ) * 180.0f / M_PI;
    float angleY = atan2(dirX, sqrt(dirY * dirY + dirZ * dirZ)) * 180.0f / M_PI;

    // Move to the starting point
    glTranslatef(branch.parent_x, branch.parent_y, branch.parent_z);

    // Draw stem
    float stemLength = length * 0.2f;
    drawCylinder(0, 0, 0, dirX * 0.1f, dirY * 0.1f, dirZ * 0.1f, 0.005f);

    // Move to the end of the stem
    glTranslatef(dirX * 0.1f, dirY * 0.1f, dirZ * 0.1f);

    // Find parent thickness
    auto parent_thickness = findParentThickness(branches, branch.parent_x, branch.parent_y, branch.parent_z);
    if (parent_thickness.first) {
        float offset = parent_thickness.second * 0.01f; // Adjust leaf position by parent thickness
        glTranslatef(offset * dirX, offset * dirY, offset * dirZ); // Apply offset
    }

    // Calculate the angle and axis for rotation
    glRotatef(-angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(90, 0, 1, 0);

    // Draw leaf
    glColor3f(0.1f, 1.0f, 0.1f);
    float leafLength = length * 0.8f;
    drawEllipse(leafLength, leafLength * 0.5f);

    glPopMatrix();
}



//check tracability to root. If not, not draw. This calc. is 54%CPU on jetson.
void drawTree(const std::vector<Branch>& branches) {
    if (branches.empty()) return;

    parentChildMap = createParentChildMap(branches);
    BranchMap = createBranchMap(branches);


    std::tuple<float, float, float> root(branches[0].parent_x, branches[0].parent_y, branches[0].parent_z);

    for (const auto& branch : branches) {
        std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
        if (canTraceBackToRoot(parentChildMap, child, root)) {
            if (branch.thickness == -1 && drawLeaves) {  // Check drawLeaves before drawing leaves
                drawLeaf(branch, branches);
            } else if (branch.thickness != -1) {
                drawCylinder(branch.parent_x, branch.parent_y, branch.parent_z,
                             branch.child_x, branch.child_y, branch.child_z,
                             branch.thickness * 0.01);
            }
        }
    }
}

void drawBranchPoints(const std::set<std::tuple<float, float, float>>& branchPoints) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for branch points
    glPointSize(10.0f); // Set point size larger
    glBegin(GL_POINTS);
    for (const auto& point : branchPoints) {
        glVertex3f(std::get<0>(point), std::get<1>(point), std::get<2>(point));
    }
    glEnd();
}
void drawTerminalBranchPoints(const std::set<std::tuple<float, float, float>>& terminalBranchPoints) {
    glColor3f(0.0f, 0.0f, 1.0f); // Red color for branch points
    glPointSize(15.0f); // Set point size larger
    glBegin(GL_POINTS);
    for (const auto& point : terminalBranchPoints) {
        glVertex3f(std::get<0>(point), std::get<1>(point), std::get<2>(point));
    }
    glEnd();
}


void drawTerminalPoints(const std::set <std::tuple<float, float, float>>& terminalPoints) {
    glColor3f(1.0f, 1.0f, 1.0f); // Red color for branch points
    glPointSize(10.0f); // Set point size larger
    glBegin(GL_POINTS);
    for (const auto& point : terminalPoints) {
        glVertex3f(std::get<0>(point), std::get<1>(point), std::get<2>(point));
    }
    glEnd();
}

// Function to remove branches with a specific parent from the branches vector
void removeBranchesWithParent(float parent_x, float parent_y, float parent_z, std::vector<Branch>& branches) {
    auto it = std::remove_if(branches.begin(), branches.end(), [&](const Branch& branch) {
        return branch.parent_x == parent_x && branch.parent_y == parent_y && branch.parent_z == parent_z;
    });

    branches.erase(it, branches.end());
}





// Modify the readCSV function to clear and update the branches vector
void updateTree(const std::string& filename, std::vector<Branch>& branches) {
    std::vector<Branch> newBranches;
    readCSV(filename, newBranches);
    branches = std::move(newBranches);



    // Find all terminal branch points
    // std::vector<std::tuple<float, float, float>> terminalBranchPoints = findTerminalBranchPoints(branchPointsMap);
    // for (const auto& point : terminalBranchPoints) {
    // std::cout << "Terminal branch point: (" << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << ")" << std::endl;
    // }

}

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

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_number>" << std::endl;
        return -1;
    }

    int file_number = std::stoi(argv[1]);
    std::string filename = "tree" + std::to_string(file_number) + ".csv";

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

    parentChildMap = createParentChildMap(branches);
    BranchMap = createBranchMap(branches);
    branchPoints = findBranchPoints(branches);
    terminalBranchPoints = findTerminalBranchPoints(BranchMap);

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



// Main loop
while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2)) {

    updateTree(filename, branches);
    // Render scene from secondary view
    renderSceneFromSecondaryView(window2);

    // Render scene from main view
    renderSceneFromMainView(window);



    // Poll for and process events
    glfwPollEvents();
}

// Cleanup and terminate GLFW
glfwDestroyWindow(window2);
glfwDestroyWindow(window);
glfwTerminate();

return 0;
}