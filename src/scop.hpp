#ifndef SCOP_HPP
# define SCOP_HPP

# include "../resources/glad/include/glad/glad.h"
# include <GLFW/glfw3.h>
# include <iostream>
# include <cmath>
# include "classes/Shader.hpp"
# include "classes/Matrix4.hpp"
# include "classes/Vec4.hpp"
# include "classes/Vec3.hpp"
# include "classes/Camera.hpp"
# include "classes/Parser.hpp"
# include "classes/Mesh.hpp"
# include "classes/BmpImage.hpp"
# include "main_utils.cpp"
const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1080;

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initializeGlfw();
#endif