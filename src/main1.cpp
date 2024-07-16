#include "classes/scop.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(Vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "scop", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	Mesh mesh = Parser::parseOBJ("resources/obj/42.obj");

	// build and compile shader program
	Shader myShader("shaders/shader1.vs", "shaders/shader1.fs");

	// // set up vertex data, buffers and configure vertex attributes
	// float vertices[] = {
	// 	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	// 	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	// 	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	// 	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	// 	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	// 	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	// 	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// 	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	// 	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	// 	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	// 	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// 	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	// 	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	// };
	// unsigned int indices[] = {
	// 	0, 1, 3,  // first Triangle
	// 	1, 2, 3   // second Triangle
	// };
	// unsigned int vbo, vao, ebo;
	// glGenVertexArrays(1, &vao);
	// glGenBuffers(1, &vbo);
	// glGenBuffers(1, &ebo);

	// // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	// glBindVertexArray(vao);

	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// // pos attr
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	// // texture attr
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	// glEnableVertexAttribArray(1);

	// // texture 1
	// unsigned int texture1, texture2;
	// glGenTextures(1, &texture1);
	// glBindTexture(GL_TEXTURE_2D, texture1);
	// // set the texture wrapping parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// // set texture filtering parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // load and generate textures
	// int width, height, nrChannels;
	// stbi_set_flip_vertically_on_load(true); 
	// unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	// if (data) {
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	// 	std::cerr << "Failed to load texture" << std::endl;
	// stbi_image_free(data);
	// // texture 2
	// glGenTextures(1, &texture2);
	// glBindTexture(GL_TEXTURE_2D, texture2);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	// if (data) {
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	// 	std::cerr << "Failed to load texture" << std::endl;
	// stbi_image_free(data);
	// myShader.use();
	// glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0); // set it manually
	// myShader.setInt("texture2", 1); // or with shader class
	// Vec3 cubePos[] = {
	// 	Vec3( 0.0f,  0.0f,  0.0f), 
	// 	Vec3( 2.0f,  5.0f, -15.0f), 
	// 	Vec3(-1.5f, -2.2f, -2.5f),  
	// 	Vec3(-3.8f, -2.0f, -12.3f),  
	// 	Vec3( 2.4f, -0.4f, -3.5f),  
	// 	Vec3(-1.7f,  3.0f, -7.5f),  
	// 	Vec3( 1.3f, -2.0f, -2.5f),  
	// 	Vec3( 1.5f,  2.0f, -2.5f), 
	// 	Vec3( 1.5f,  0.2f, -1.5f), 
	// 	Vec3(-1.3f,  1.0f, -1.5f)
	// };

	// // //Camera
	// // Vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	// // Vec3 cameraDirection(cameraPos - cameraTarget);
	// // cameraDirection.normalize();
	// // Vec3 up(0.0f, 1.0f, 0.0f);
	// // Vec3 cameraRight;
	// // cameraRight = cameraRight.cross(up, cameraDirection);
	// // cameraRight.normalize();
	// // Vec3 cameraUp;
	// // cameraUp = cameraUp.cross(cameraDirection, cameraRight);

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
		// // bind Texture
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);
		myShader.use();

		// pass projection matrix to shader
		Matrix4 projection;
		projection.perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		unsigned int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.getValuePtr());

		// Apply transformation on the shader
		unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
		// Camera/view tranformation
		Matrix4 view;
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.getValuePtr());

		Matrix4 model;
		model.translate(Vec3(0.0f, 0.0f, 0.0f));
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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
