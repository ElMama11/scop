#include "includes/scop.hpp"

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
		//normalizeAndCenterMesh(mesh);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		exit (0);
	}
	return mesh;
}