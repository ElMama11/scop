#include "scop.hpp"
// Screen
const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1080;

//Transformation
float rotationAngle = 0.0f;

// Camera
Camera camera(Vec3(0.0f, 0.0f, 6.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool freeCamera = false;
bool cKeyPressed = false;

// Frame logic
float deltaTime = 0.5f;
float lastFrame = 0.0f;

// Texture switch
bool useTexture = false;
bool tKeyPressed = false;
bool transitioning = false;
float transitionFactor = 0.0f;

// Wireframe switch
bool zKeyPressed = false;
bool wireframe = false;

// Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* initializeGlfw() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "scop", NULL, NULL);
	if (window == NULL) {
		throw std::runtime_error("Failed to create GLFW window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD");
	glEnable(GL_DEPTH_TEST);
	return window;
}

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

unsigned int buildTexture(char **av) {

	std::string texPathSuffix = av[2];
	std::string texPath = "resources/textures/" + texPathSuffix + ".bmp";
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate texture
	BmpImage image(texPath);
	if (image.isValid()) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else
		std::cerr << "Failed to load texture" << std::endl;
	return texture;
}

Mesh parseMesh(char **av) {
	std::string objPathSuffix = av[1];
	std::string objPath = "resources/obj/" + objPathSuffix + ".obj";
	Mesh mesh;
	try {
		mesh = Parser::parseOBJ(objPath);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		exit (0);
	}
	return mesh;
}

void smoothTextureTransition(Shader myShader) {
	if (transitioning) {
		if (useTexture) {
			transitionFactor += deltaTime;
			if (transitionFactor >= 1.0f) {
				transitionFactor = 1.0f;
				transitioning = false;
			}
		}
		else {
			transitionFactor -= deltaTime;
			if (transitionFactor <= 0.0f) {
				transitionFactor = 0.0f;
				transitioning = false;
			}
		}
	}
	myShader.setFloat("transitionFactor", transitionFactor);
}

void bindTexture(unsigned int texture) {
	if (useTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

Vec3 calculateObjectCenter(const Mesh& mesh) {
    Vec3 center(0.0f, 0.0f, 0.0f);

    for (const Vertex& vertex : mesh.vertices) {
        center = center + vertex.position;  // Accumulate positions
    }

    // Divide by the number of vertices to get the average (center of the object)
    if (!mesh.vertices.empty()) {
        center = center / static_cast<float>(mesh.vertices.size());
    }

    return center;
}


void applyTransformations(Shader myShader, Mesh mesh) {
	// Get uniform variable in the Vshader
	unsigned int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
	unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
	// Pass projection matrix to shader
	Matrix4 projection;
	projection.perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.getValuePtr());
	// Camera/view tranformation
	Matrix4 view;
	view = camera.GetViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.getValuePtr());
	// Apply transformation on the object
	Matrix4 model;
	 // Step 1: Calculate the center of the object
    Vec3 center = calculateObjectCenter(mesh);

    // Step 2: Translate the object to the origin (center becomes the pivot point)
    model.translate(Vec3(-center.x, -center.y, -center.z));

    // Step 3: Rotate the object around its new origin (previously its center)
    rotationAngle += 0.5f;
    model.rotate(rotationAngle, 0.0f, 1.0f, 0.0f);

    // Step 4: Translate the object back to its original position
    model.translate(center);

	// Optional: apply scaling
	model.scale(Vec3(1.0f, 1.0f, 1.0f));
	// model.translate(Vec3(0.0f, 0.0f, 0.0f));
	// rotationAngle += 0.5f;
	// model.rotate(rotationAngle, 0.0f, 1.0f, 0.0f);
	// model.scale(Vec3(1.0f, 1.0f, 1.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.getValuePtr());
}

void perFrameLogic() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;  
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

void clearWindow() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}