#include "includes/scop.hpp"

int main(int ac, char **av) {
	if (av[1] == NULL || av[2] == NULL || ac != 3) {
		std::cerr << "Args : [object] [texture]\n\nExemple : ./scop backpack backpack\n\nInput : WASD = Move the obj\n\tZ = Wireframe mode\n\tT = Texturing\n\tC = Free camera\n\tESC = Quit" << std::endl;
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
	Mesh mesh = parseMesh(av);
	unsigned int texture = buildTexture(av);
	// Build and compile shader program
	Shader myShader("shaders/shader1.vs", "shaders/shader1.fs");
	myShader.setInt("ourTexture", 0);
	renderLoop(window, myShader, texture, mesh);
	glfwTerminate();
	return 0;
}