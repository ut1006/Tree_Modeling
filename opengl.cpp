#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

//g++ opencv.cpp -o 3d_cylinder -lglfw -lGLEW -lGL -lGLU


// Rotation angles
float rotationX = 0.0f;
float rotationY = 0.0f;

// Zoom level
float zoom = 1.0f;

// Viewpoint position
float posX = 0.0f;
float posZ = 0.0f;

// Mouse drag state
bool dragging = false;
double lastMouseX = 0.0, lastMouseY = 0.0;

// Mouse callback to update rotation angles and perform linear movement with Ctrl key
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool ctrlPressed = false; // Track Ctrl key state
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
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

void drawCylinder() {
    GLUquadric* quad = gluNewQuadric();
    
    // Set the color to brown
    glColor3f(0.65f, 0.16f, 0.16f);

    // Draw the cylinder
    glPushMatrix();
    gluCylinder(quad, 1.0, 1.0, 2.0, 32, 32);

    // Draw the top circle
    glTranslatef(0.0f, 0.0f, 2.0f);
    gluDisk(quad, 0.0, 1.0, 32, 1);

    // Draw the bottom circle
    glTranslatef(0.0f, 0.0f, -2.0f);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 1.0, 32, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);

    // Draw edges
    glColor3f(0.0f, 0.0f, 0.0f); // Set edge color to black
    glBegin(GL_LINES);

    // Bottom circle edges
    for (int i = 0; i < 32; ++i) {
        float theta1 = (float)i * 2.0f * M_PI / 32.0f;
        float theta2 = (float)(i + 1) * 2.0f * M_PI / 32.0f;
        glVertex3f(cos(theta1), sin(theta1), 0.0f);
        glVertex3f(cos(theta2), sin(theta2), 0.0f);
    }

    // Top circle edges
    for (int i = 0; i < 32; ++i) {
        float theta1 = (float)i * 2.0f * M_PI / 32.0f;
        float theta2 = (float)(i + 1) * 2.0f * M_PI / 32.0f;
        glVertex3f(cos(theta1), sin(theta1), 2.0f);
        glVertex3f(cos(theta2), sin(theta2), 2.0f);
    }

    // Vertical edges
    for (int i = 0; i < 32; ++i) {
        float theta = (float)i * 2.0f * M_PI / 32.0f;
        glVertex3f(cos(theta), sin(theta), 0.0f);
        glVertex3f(cos(theta), sin(theta), 2.0f);
    }

    glEnd();
}

void drawGroundPlane() {
    glColor3f(0.5f, 0.5f, 0.5f); // Set the ground plane color to gray
    glBegin(GL_QUADS);

    // Draw a large ground plane
    glVertex3f(-10.0f, -1.0f, -10.0f);
    glVertex3f( 10.0f, -1.0f, -10.0f);
    glVertex3f( 10.0f, -1.0f,  10.0f);
    glVertex3f(-10.0f, -1.0f,  10.0f);

    glEnd();
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Cylinder", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
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
        gluLookAt(3.0 * zoom + posX, 3.0 * zoom, 3.0 * zoom + posZ, posX, 0.0, posZ, 0.0, 1.0, 0.0);

        // Apply rotations
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

        // Draw XYZ axes
        drawAxes();

        // Draw the ground plane
        drawGroundPlane();

        // Draw the cylinder
        drawCylinder();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
