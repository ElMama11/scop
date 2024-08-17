#include "scop.hpp"

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
	Mesh mesh = parseMesh(av);
	unsigned int texture = buildTexture(av);
	// Build and compile shader program
	Shader myShader("shaders/shader1.vs", "shaders/shader1.fs");
	myShader.setInt("ourTexture", 0);
	
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		perFrameLogic();
		processInput(window);
		clearWindow();
		myShader.use();
		smoothTextureTransition(myShader);
		bindTexture(texture);
		applyTransformations(myShader);
		mesh.draw(myShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}