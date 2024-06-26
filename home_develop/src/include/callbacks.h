#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
