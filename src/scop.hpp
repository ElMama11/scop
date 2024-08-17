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

/* main_utils.cpp */
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
GLFWwindow* initializeGlfw();
unsigned int buildTexture(char **av);
Mesh parseMesh(char **av);
void smoothTextureTransition(Shader myShader);
void bindTexture(unsigned int texture);
void applyTransformations(Shader myShader, Mesh mesh);
void perFrameLogic();
void processInput(GLFWwindow *window);
void clearWindow();
#endif