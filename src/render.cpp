#include "includes/scop.hpp"

float deltaTime = 0.5f;
float lastFrame = 0.0f;
float rotationAngle = 0.0f;
float transitionFactor = 0.0f;
bool transitioning = false;

void smoothTextureTransition(Shader myShader) {
	if (transitioning) {
		if (useTexture) {
			transitionFactor += deltaTime;
			if (transitionFactor >= 1.0f) {
				transitionFactor = 1.0f;
				transitioning = false;
			}
		}
		else {
			transitionFactor -= deltaTime;
			if (transitionFactor <= 0.0f) {
				transitionFactor = 0.0f;
				transitioning = false;
			}
		}
	}
	myShader.setFloat("transitionFactor", transitionFactor);
}

void applyTransformations(Shader myShader, const Mesh& mesh) {
	// Get uniform variables in the vertex shader
	unsigned int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
	unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");

	// Pass projection matrix to shader
	Matrix4 projection;
	projection.perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.getValuePtr());
	// Camera/view transformation
	Matrix4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.getValuePtr());
	// Calculate the bounding box of the object
	Vec3 min = mesh.vertices[0].position;
	Vec3 max = mesh.vertices[0].position;
	for (const auto& vertex : mesh.vertices) {
		min.x = std::min(min.x, vertex.position.x);
		min.y = std::min(min.y, vertex.position.y);
		min.z = std::min(min.z, vertex.position.z);
		max.x = std::max(max.x, vertex.position.x);
		max.y = std::max(max.y, vertex.position.y);
		max.z = std::max(max.z, vertex.position.z);
	}
	// Calculate the center and size of the object
	Vec3 center = (min + max) * 0.5f;
	Vec3 size = max - min;
	float maxDim = std::max(size.x, std::max(size.y, size.z));
	// Calculate scale factor to fit within [-1, 1]
	float scale = 4.0f / maxDim;

	Matrix4 model;
	// Scale the object to fit within [-1, 1]
	model.scale(Vec3(scale, scale, scale));
	// Translate to center
	model.translate(Vec3(-center.x, -center.y, -center.z));
	// Apply rotation
	rotationAngle += 0.5f;
	model.rotate(rotationAngle, 0.0f, 1.0f, 0.0f);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.getValuePtr());
}

Vec3 calculateObjectCenter(const Mesh& mesh) {
	Vec3 center(0.0f, 0.0f, 0.0f);
	for (const Vertex& vertex : mesh.vertices)
		center = center + vertex.position;  // Accumulate positions
	// Divide by the number of vertices to get the average (center of the object)
	if (!mesh.vertices.empty())
		center = center / static_cast<float>(mesh.vertices.size());
	return center;
}

void bindTexture(unsigned int texture) {
	if (useTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

void perFrameLogic() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;  
}

void clearWindow() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderLoop(GLFWwindow* window, Shader myShader, unsigned int texture, Mesh mesh) {
	while (!glfwWindowShouldClose(window)) {
		perFrameLogic();
		processInput(window);
		clearWindow();
		myShader.use();
		smoothTextureTransition(myShader);
		bindTexture(texture);
		applyTransformations(myShader, mesh);
		mesh.draw(myShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}