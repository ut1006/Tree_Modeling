#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

// Structure to hold the node information
struct Node {
    std::string type;
    int id;
    float posx=0, posy=0, posz=0;
    float dirx=0, diry=0, dirz=0;
    float length=0, width=0;
};

struct CSR {
    std::vector<int> values;
    std::vector<int> col_indices;
    std::vector<int> row_pointers;
};

// Structure to hold the branch information
struct Branch {
    float parent_x, parent_y, parent_z;
    float child_x, child_y, child_z;
    float thickness;
};

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
        std::cout << branch.parent_x << ", " << branch.parent_y << ", " << branch.parent_z << ", "
            << branch.child_x << ", " << branch.child_y << ", " << branch.child_z << ", "
            << branch.thickness << std::endl;
    }
}

void displayData(const std::vector<Branch>& branches) {
    std::cout << "Branches:" << std::endl;
    for (const auto& branch : branches) {
        std::cout << branch.parent_x << ", " << branch.parent_y << ", " << branch.parent_z << ", "
                  << branch.child_x << ", " << branch.child_y << ", " << branch.child_z << ", "
                  << branch.thickness << std::endl;
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
std::vector<Node> nodes;
CSR csr;
std::vector<Branch> branches;

// Mouse callback to update rotation angles
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool ctrlPressed = false; // Track Ctrl key state
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS|| 
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

    // Calculate the cylinder's height
    float height = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));

    // Set the color to brown
    glColor3f(0.4f, 0.1f, 0.1f);

    // Draw the cylinder
    glPushMatrix();
    glTranslatef(x1, y1, z1);
    gluQuadricOrientation(quad, GLU_OUTSIDE);
    gluCylinder(quad, radius, radius, height, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawTree(const std::vector<Branch>& branches) {
    for (const auto& branch : branches) {
        drawCylinder(branch.parent_x, branch.parent_y, branch.parent_z,
                     branch.child_x, branch.child_y, branch.child_z,
                     0.01);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // Load the tree data
    readCSV("sca_tree.csv", branches);

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

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Set up camera
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(3.0 * zoom + posX + 2, 6.0 * zoom, 3.0 * zoom + posZ +5, posX, 3.0, posZ, 0.0, 1.0, 0.0);

        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

        // Draw XYZ axes
        drawAxes();

        // Draw the ground plane
        drawGroundPlane();

        drawTree(branches);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
