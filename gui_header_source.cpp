//gui_header_source.cpp
#include "gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
float rotationX = 0.0f;
float rotationY = 0.0f;
float zoom = 1.0f;
float posX = 0.0f;
float posY = 0.0f;
float posZ = 0.0f;
bool dragging = false;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool drawLeaves = false;
GLFWwindow* window = nullptr;
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
