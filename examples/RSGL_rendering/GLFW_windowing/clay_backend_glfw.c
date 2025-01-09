#include <GLFW/glfw3.h>

#define GLFW_UNUSED(x) (void)(x);

Clay_Vector2 GLFW_mousePosition;

u32 GLFW_mouseLeft = 0;

GLFWkeyfun GLFW_old_key = NULL; 
GLFWcharfun GLFW_old_char = NULL;
GLFWwindowsizefun GLFW_old_resize = NULL;
GLFWmousebuttonfun GLFW_old_button = NULL;
GLFWcursorposfun GLFW_old_cursor = NULL;
GLFWscrollfun GLFW_old_scroll = NULL;


void windowSizeCallback(GLFWwindow *window, int width, int height) {
    GLFW_UNUSED(window)
    Clay_SetLayoutDimensions((Clay_Dimensions) { (float)width, (float)height });

	if (GLFW_old_resize != NULL)
		GLFW_old_resize(window, width, height);
}

static void charCallback(GLFWwindow *window, unsigned int codepoint) {
    GLFW_UNUSED(window)

	if (GLFW_old_char != NULL)
		GLFW_old_char(window, codepoint);
}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLFW_UNUSED(window)
    GLFW_UNUSED(mods)
    GLFW_UNUSED(scancode)
	
	
	if (GLFW_old_key != NULL)
		GLFW_old_key(window, key, scancode, action, mods);

}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    GLFW_UNUSED(window)
    GLFW_UNUSED(mods)

    if (button == GLFW_MOUSE_BUTTON_LEFT)
        GLFW_mouseLeft = action == GLFW_PRESS ? 1 : 0; 

    Clay_SetPointerState(GLFW_mousePosition, GLFW_mouseLeft);
	if (GLFW_old_cursor != NULL)
		GLFW_old_button(window, button, action, mods);
}

static void mouseCursorPosCallback(GLFWwindow *window, double x, double y) {
    GLFW_UNUSED(window);
	
    GLFW_mousePosition = (Clay_Vector2){ (float)x, (float)y };
    Clay_SetPointerState(GLFW_mousePosition, GLFW_mouseLeft);
	if (GLFW_old_cursor != NULL)
		GLFW_old_cursor(window, x, y);
}

static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    GLFW_UNUSED(window)

    Clay_UpdateScrollContainers(
        false,
        (Clay_Vector2) { xoffset, yoffset },
        0
    );
	
	if (GLFW_old_scroll != NULL)
		GLFW_old_scroll(window, xoffset, yoffset);
}

void clay_GLFW_callbackInit(GLFWwindow* window) {
	GLFW_old_key = glfwSetKeyCallback(window, keyCallback);
    GLFW_old_char = glfwSetCharCallback(window, charCallback);
    GLFW_old_resize = glfwSetWindowSizeCallback(window, windowSizeCallback);
    GLFW_old_button = glfwSetMouseButtonCallback(window, mouseButtonCallback);
    GLFW_old_cursor = glfwSetCursorPosCallback(window, mouseCursorPosCallback);
    GLFW_old_scroll = glfwSetScrollCallback(window, mouseScrollCallback);
}