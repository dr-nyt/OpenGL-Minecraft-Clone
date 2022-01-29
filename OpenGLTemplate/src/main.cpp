#include "core.h"
#include "engine/window.h"
#include "engine/input.h"
#include "engine/shader.h"
#include "engine/buffers.h"

using namespace Engine;

void terminateGLFW();

int main() {
	const int windowWidth = 1920;
	const int windowHeight = 1080;
	const bool fullScreenMode = false;

	// Create Window
	const bool success = Window::createWindow(windowWidth, windowHeight, "OpenGL Template", fullScreenMode);
	if (!success) return -1;

	// Initialize shader
	// Remember to delete shaders created this way at the end
	Shader* shader = NULL;
	try {
		shader = new Shader("assets/shaders/vertexShader.glsl", "assets/shaders/fragmentShader.glsl");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		terminateGLFW();
		return -1;
	}

	// Create vertices for a square
	// Update Vertex in shader.h to add more attributes
	Vertex vertices[] = {
		// Postion							// Color
		{ glm::vec3(0.5f, -0.5f, 0.0f),		glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) },	// 0 Bottom Right
		{ glm::vec3(0.5f, 0.5f, 0.0f),		glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 1 Top Right
		{ glm::vec3(-0.5f, 0.5f, 0.0f),		glm::vec4(0.8f, 0.2f, 0.9f, 1.0f) },	// 2 Top Left
		{ glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec4(0.8f, 0.9f, 0.2f, 1.0f) },	// 3 Bottom Left
	};
	// Automaticall calculate required data
	GLuint vertexLen = sizeof(Vertex) / sizeof(float);
	GLsizeiptr verticesByteSize = sizeof(vertices);
	GLuint vertexCount = (GLuint)(verticesByteSize / vertexLen / sizeof(float));
	// Set usage type GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc.
	GLenum usage = GL_STATIC_DRAW;

	// Create the indices
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	GLuint indicesByteSize = sizeof(indices);
	GLuint indicesLen = (GLuint)(indicesByteSize / sizeof(GLuint));

	// Create VAO, VBO, EBO & set attributes
	GLuint vaoID = Buffers::createVAO();
	GLuint bindingIndex = 0;
	Buffers::createVBO(vaoID, verticesByteSize, vertices, bindingIndex, vertexLen, usage);
	Buffers::createEBO(vaoID, indicesByteSize, indices, usage);
	Buffers::addVertexAttrib(vaoID, 0, 3, offsetof(Vertex, position), bindingIndex);		// Position
	Buffers::addVertexAttrib(vaoID, 1, 4, offsetof(Vertex, color), bindingIndex);		// Color

	// Transform matrix
	glm::vec3 scale = glm::vec3(5.0f);
	float rotation = 0.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 transformMatrix = glm::scale(glm::mat4(1.0f), scale);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	transformMatrix = glm::translate(transformMatrix, position);

	// View matrix
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 center = glm::vec3(0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);

	// Projection matrix
	float projectionWidth = 1920.0f;
	float windowAspect = ((float)Window::windowWidth / (float)Window::windowHeight);
	float projectionHeight = projectionWidth / windowAspect;

	float fov = 45.0f;
	float left = -projectionWidth / 2.0f;
	float right = projectionWidth / 2.0f;
	float bottom = -projectionHeight / 2.0f;
	float top = projectionHeight / 2.0f;
	float near = 0.0001f;
	float far = 10000.0f;

	//glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
	glm::mat4 projectionMatrix = glm::perspective(fov, windowAspect, near, far);


	// Set clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Main loop
	while (!glfwWindowShouldClose(Window::nativeWindow)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle input
		Input::handleKeyInput(transformMatrix);

		// Render
		Buffers::useVAO(vaoID);
		shader->use();
		shader->setMat4("uTransform", transformMatrix);
		shader->setMat4("uView", viewMatrix);
		shader->setMat4("uProjection", projectionMatrix);
		//glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glDrawElements(GL_TRIANGLES, indicesLen, GL_UNSIGNED_INT, 0);

		// Swap buffers & Handle window events
		glfwSwapBuffers(Window::nativeWindow);
		glfwPollEvents();
	}

	// Terminate
	delete shader;
	terminateGLFW();
	return 0;
}

void Input::handleKeyInput(glm::mat4 &t) {
	if (Input::isKeyDown(GLFW_KEY_ESCAPE)) {
		Window::close();
	}
	if (Input::isKeyDown(GLFW_KEY_W)) {
		t = glm::translate(t, glm::vec3(0.0f, 0.1f, 0.0f));
	}
	if (Input::isKeyDown(GLFW_KEY_S)) {
		t = glm::translate(t, glm::vec3(0.0f, -0.1f, 0.0f));
	}
	if (Input::isKeyDown(GLFW_KEY_A)) {
		t = glm::translate(t, glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	if (Input::isKeyDown(GLFW_KEY_D)) {
		t = glm::translate(t, glm::vec3(0.1f, 0.0f, 0.0f));
	}
}

void terminateGLFW() {
	glfwDestroyWindow(Window::nativeWindow);
	glfwTerminate();
}