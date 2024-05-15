#ifndef SCOP_HPP
# define SCOP_HPP

# include "../../ressources/glad/include/glad/glad.h"
# include <GLFW/glfw3.h>
# include <iostream>
# include <cmath>
# include "Shader.hpp"
# include "stb_image.hpp"
// #include "../glfw-3.4/deps/linmath.h"

void processInput(GLFWwindow *window, Shader myShader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif