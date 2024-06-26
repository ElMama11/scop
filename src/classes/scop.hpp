#ifndef SCOP_HPP
# define SCOP_HPP

# include "../../ressources/glad/include/glad/glad.h"
# include <GLFW/glfw3.h>
# include <iostream>
# include <cmath>
# include "Shader.hpp"
# include "stb_image.hpp"
# include "Matrix4.hpp"
# include "Vec4.hpp"
# include "Vec3.hpp"
# include "Camera.hpp"
// #include "../glfw-3.4/deps/linmath.h"

void processInput(GLFWwindow *window, Shader myShader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

#endif