#ifndef SCOP_HPP
# define SCOP_HPP

# include "../../resources/glad/include/glad/glad.h"
# include <GLFW/glfw3.h>
# include <iostream>
# include <cmath>

# include "../classes/Shader.hpp"
# include "../classes/Matrix4.hpp"
# include "../classes/Vec4.hpp"
# include "../classes/Vec3.hpp"
# include "../classes/Camera.hpp"
# include "../classes/Parser.hpp"
# include "../classes/Mesh.hpp"
# include "../classes/BmpImage.hpp"

// Screen
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

//Transformation
extern float rotationAngle;

// Camera
extern Camera camera;
extern bool firstMouse;
extern float lastX;
extern float lastY;
extern bool freeCamera;
extern bool cKeyPressed;

// Frame logic
extern float deltaTime;
extern float lastFrame;

// Texture switch
extern bool useTexture;
extern bool tKeyPressed;
extern bool transitioning;
extern float transitionFactor;

// Wireframe switch
extern bool zKeyPressed;
extern bool wireframe;

/* init.cpp*/
GLFWwindow* initializeGlfw();
unsigned int buildTexture(char **av);
Mesh parseMesh(char **av);
/* hooks.cpp */
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
/* render.cpp */
void applyTransformations(Shader myShader, const Mesh& mesh);
void renderLoop(GLFWwindow* window, Shader myShader, unsigned int texture, Mesh mesh);
void smoothTextureTransition(Shader myShader);
void bindTexture(unsigned int texture);
Vec3 calculateObjectCenter(const Mesh& mesh);
void clearWindow();
void perFrameLogic();
#endif