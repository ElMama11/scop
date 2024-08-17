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
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;
extern Camera camera;

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
GLFWwindow* initializeGlfw();
#endif