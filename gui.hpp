#ifdef GUI
#define GUI
// gui.hpp
extern float rotationX;
extern float rotationY;
extern float zoom;
extern float posX;
extern float posY;
extern float posZ;
extern bool dragging;
extern double lastMouseX,lastMouseY ;
extern bool drawLeaves ;
extern  GLFWwindow* window ; 

// Function prototypes
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif 