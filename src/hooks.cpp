#include "includes/scop.hpp"

// Screen
const unsigned int SCR_HEIGHT = 1080;
const unsigned int SCR_WIDTH = 1920;
// Texture switch
bool useTexture = false;
bool tKeyPressed = false;
// Wireframe switch
bool zKeyPressed = false;
bool wireframe = false;
// Camera
Camera camera(Vec3(0.0f, 0.0f, 6.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool freeCamera = false;
bool cKeyPressed = false;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(BOTTOM, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !tKeyPressed) {
		tKeyPressed = true;
		transitioning = true;
		useTexture = !useTexture;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
		tKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && zKeyPressed == false) {
		zKeyPressed = true;
		if (wireframe == false) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = true;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = false;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
		zKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && cKeyPressed == false) {
		cKeyPressed = true;
		if (freeCamera == false) {
			glfwSetCursorPosCallback(window, mouse_callback);
			freeCamera = true;
		}
		else {
			glfwSetCursorPosCallback(window, NULL);
			freeCamera = false;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
		cKeyPressed = false;
}