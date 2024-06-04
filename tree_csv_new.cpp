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

// Function to read the CSV file
void readCSV(const std::string& filename, std::vector<Node>& nodes, CSR& csr) {
    std::ifstream file(filename);
    std::string line;
    
    // Skip the header
    std::getline(file, line); 
    
    // Read node data
    while (std::getline(file, line)) {
        if (line[0] == 'b' || line[0] == 'l') {
            std::stringstream ss(line);
            Node node;
            char type;
            ss >> type;
            ss.ignore(1);
            ss>> node.id;
            ss.ignore(1);
            ss >> node.posx;
            ss.ignore(1);
            ss >> node.posy;
            ss.ignore(1);
            ss >> node.posz;
            ss.ignore(1);
            ss >> node.dirx;
            ss.ignore(1);
            ss >> node.diry;
            ss.ignore(1);
            ss >> node.dirz;
            ss.ignore(1);
            ss >> node.length;
            ss.ignore(1);
            ss >> node.width;
            node.type = type;
            nodes.push_back(node);
        } else {
            break;
        }
    }
    
    // Read CSR values
    {
        std::stringstream ss(line);
        std::string temp;
        int value;
        while (ss >> value) {
            csr.values.push_back(value);
            if (ss.peek() == ',') ss.ignore();
        }
    }
    
    // Read CSR col_indices
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string temp;
        int value;
        while (ss >> value) {
            csr.col_indices.push_back(value);
            if (ss.peek() == ',') ss.ignore();
        }
    }
    
    // Read CSR row_pointers
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string temp;
        int value;
        while (ss >> value) {
            csr.row_pointers.push_back(value);
            if (ss.peek() == ',') ss.ignore();
        }
    }
}


void displayData(const std::vector<Node>& nodes, const CSR& csr) {
    std::cout << "Nodes:" << std::endl;
    for (const auto& node : nodes) {
        std::cout << node.type << ", " << node.id << ", " << node.posx << ", " << node.posy << ", " << node.posz << ", "
                  << node.dirx << ", " << node.diry << ", " << node.dirz << ", " << node.length << ", " << node.width << std::endl;
    }

    std::cout << "CSR values:" << std::endl;
    for (const auto& value : csr.values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "CSR col_indices:" << std::endl;
    for (const auto& col_index : csr.col_indices) {
        std::cout << col_index << " ";
    }
    std::cout << std::endl;

    std::cout << "CSR row_pointers:" << std::endl;
    for (const auto& row_pointer : csr.row_pointers) {
        std::cout << row_pointer << " ";
    }
    std::cout << std::endl;
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
    glVertex3f(-10.0f, -1.0f, -10.0f);
    glVertex3f(10.0f, -1.0f, -10.0f);
    glVertex3f(10.0f, -1.0f, 10.0f);
    glVertex3f(-10.0f, -1.0f, 10.0f);

    glEnd();
}

void drawCylinder(float length, float radius) {
    GLUquadric* quad = gluNewQuadric();

    // Set the color to brown
    glColor3f(0.4f, 0.1f, 0.1f);

    // Draw the cylinder
    gluCylinder(quad, radius, radius, length, 32, 32);

    // Set the color to brown
    glColor3f(0.7f, 0.35f, 0.2f);


    // Draw the top circle
    glTranslatef(0.0f, 0.0f, length);
    gluDisk(quad, 0.0, radius, 32, 1);

    // Draw the bottom circle
    glTranslatef(0.0f, 0.0f, -length);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, radius, 32, 1);

    gluDeleteQuadric(quad);

    glTranslatef(0.0f, 0.0f, -length);

    // Draw edges
    glColor3f(0.0f, 0.0f, 0.0f); // Set edge color to black
    glBegin(GL_LINES);

    // Bottom circle edges
    for (int i = 0; i < 32; ++i) {
        float theta1 = (float)i * 2.0f * M_PI / 32.0f;
        float theta2 = (float)(i + 1) * 2.0f * M_PI / 32.0f;
        glVertex3f(cos(theta1) * radius, sin(theta1) * radius, 0.0f);
        glVertex3f(cos(theta2) * radius, sin(theta2) * radius, 0.0f);
    }

    // Top circle edges
    for (int i = 0; i < 32; ++i) {
        float theta1 = (float)i * 2.0f * M_PI / 32.0f;
        float theta2 = (float)(i + 1) * 2.0f * M_PI / 32.0f;
        glVertex3f(cos(theta1) * radius, sin(theta1) * radius, length);
        glVertex3f(cos(theta2) * radius, sin(theta2) * radius, length);
    }

    // Vertical edges
    for (int i = 0; i < 32; ++i) {
        float theta = (float)i * 2.0f * M_PI / 32.0f;
        glVertex3f(cos(theta) * radius, sin(theta) * radius, 0.0f);
        glVertex3f(cos(theta) * radius, sin(theta) * radius, length);
    }

    glEnd();
}

void drawBranch(const Node& node) {
    glPushMatrix();
   
    // Apply translation and rotation based on the node's position and direction
    glTranslatef(node.posx, node.posy, node.posz);
    float angleX = -atan2(node.diry, node.dirz) * 180.0 / M_PI;
    float angleY = atan2(node.dirx, node.dirz) * 180.0 / M_PI;
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    drawCylinder(node.length, node.width);
    glPopMatrix();
}


void drawTree(const std::vector<Node>& nodes, const CSR& csr) {
    std::unordered_map<int, std::vector<int>> parentToChildren;

    // 親ノードに対してその子ノードのIDをマッピング
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (int j = csr.row_pointers[i]; j < csr.row_pointers[i + 1]; ++j) {
            int childIndex = csr.col_indices[j];
            if (childIndex >= 0 && childIndex < nodes.size()) {
                parentToChildren[nodes[i].id].push_back(childIndex);
            }
        }
    }

    // ノードを描画し、その子ノードも描画
    for (const auto& node : nodes) {
        drawBranch(node);
        if (parentToChildren.find(node.id) != parentToChildren.end()) {
            for (const int childIndex : parentToChildren[node.id]) {
                drawBranch(nodes[childIndex]);
            }
        }
    }
}
int main() {
    // Load the tree data
    readCSV("new_tree_info.csv", nodes, csr);

    // Display loaded data for debugging
    displayData(nodes, csr);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Tree Visualization", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set callback functions
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // Load the tree data
    readCSV("new_tree_info.csv", nodes, csr);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set up viewport and projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    // Set the mouse, scroll, and key callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up camera
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //For 3 seconds provides round view
        double time = glfwGetTime();
        float eyeX = 0.0f;
        float eyeY = 0.0f;
        float eyeZ = 0.0f;

        if (time < 3.0) {
            eyeX = 5 * cos(2 * time * M_PI / 3.0); // Calculate eye X position
            eyeY = 10.0f * zoom; // Eye Y position remains unchanged
            eyeZ = 5 * sin(2 * time * M_PI / 3.0); // Calculate eye Z position
            gluLookAt(posX+eyeX, eyeY, posZ+eyeZ, posX, 0.0, posZ, 0.0, 1.0, 0.0);
        }

        //After 3 sec GUI 
        if(time>3.0){
        gluLookAt(3.0 * zoom + posX + 2, 6.0 * zoom, 3.0 * zoom + posZ +5, posX, 3.0, posZ, 0.0, 1.0, 0.0);
        }

        // Apply rotations
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

        // Draw XYZ axes
        drawAxes();

        // Draw the ground plane
        drawGroundPlane();



        // Draw the tree
        drawTree(nodes, csr);

        

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
