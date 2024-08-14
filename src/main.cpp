#include "scop.hpp"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;
float rotationAngle = 0.0f;

// Texture switch
bool useTexture = false;
bool tKeyPressed = false;
bool transitioning = false;
float transitionFactor = 0.0f;

// Wireframe switch
bool zKeyPressed = false;
bool wireframe = false;

// Camera
Camera camera(Vec3(0.0f, 0.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool freeCamera = false;
bool cKeyPressed = false;

float deltaTime = 0.5f;
float lastFrame = 0.0f;

int main(int ac, char **av) {
	if (av[1] == NULL || av[2] == NULL || ac != 3) {
		std::cerr << "args : [object] [texture]\n\nInput : WASD = Move the obj\n\tZ = Wireframe mode\n\tT = Texturing" << std::endl;
		return 0;
	}
	GLFWwindow* window;
	try {
        window = initializeGlfw();
    }
	catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
	std::string objPathSuffix = av[1];
	std::string texPathSuffix = av[2];
	std::string objPath = "resources/obj/" + objPathSuffix + ".obj";
	std::string texPath = "resources/textures/" + texPathSuffix + ".bmp";
	Mesh mesh;
	try {
        mesh = Parser::parseOBJ(objPath);
    }
	catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
	std::cout << texPath << std::endl;

	// build and compile shader program
	Shader myShader("shaders/shader1.vs", "shaders/shader1.fs");

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
	}
	else
		std::cerr << "Failed to load texture" << std::endl;

	myShader.use();
	myShader.setInt("ourTexture", 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per frame logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;  

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		myShader.use();

		 // Smooth transition for textures
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

		// Bind texture if using texture
		if (useTexture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}

		unsigned int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");

		// pass projection matrix to shader
		Matrix4 projection;
		projection.perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.getValuePtr());
		// Camera/view tranformation
		Matrix4 view;
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.getValuePtr());

		Matrix4 model;
		model.translate(Vec3(0.0f, 0.0f, 0.0f));
		rotationAngle += 0.5f;
		model.rotate(rotationAngle, 0.0f, 1.0f, 0.0f);
		model.scale(Vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.getValuePtr());

		mesh.draw(myShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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