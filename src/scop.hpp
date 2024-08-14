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

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
Vec3 calculateCenter(const std::vector<Vertex>& vertices);

#endif