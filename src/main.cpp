#include "core.h"
#include "window/window.h"
#include "window/input.h"
#include "shader.h"

using namespace MinecraftClone;

int main() {
	const int windowWidth = 1920;
	const int windowHeight = 1080;
	const bool fullScreenMode = false;

	// Create Window
	const bool success = Window::createWindow(windowWidth, windowHeight, "Minecraft Clone", fullScreenMode);
	if (!success) return -1;

	// Initialize shader
	Shader* shader = NULL;
	try { shader = new Shader("assets/shaders/vertexShader.glsl", "assets/shaders/fragmentShader.glsl"); }
	catch (std::exception& e) { std::cout << e.what() << std::endl; return -1; }

	// Create Vertices
	Vertex vertices[] = {
		// Top
		{ glm::vec3(0.0f, 0.75f, 0.0f),  glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) },	// 0 Top
		{ glm::vec3(-0.1f, 0.5f, 0.0f),   glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 1 Left
		{ glm::vec3(0.1f,  0.5f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 2 Right
		// Top Right
		{ glm::vec3(0.35f,  0.5f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 3 Right
		{ glm::vec3(0.2f,  0.25f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 4 Bottom
		// Bottom Right
		{ glm::vec3(0.3f,  -0.15f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 5 Bottom
		{ glm::vec3(0.0f,  0.1f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 6 Left
		// Bottom Left
		{ glm::vec3(-0.3f,  -0.15f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 7 Bottom
		{ glm::vec3(-0.2f,  0.25f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 8 top
		// Top Left
		{ glm::vec3(-0.35f,  0.5f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },	// 9 Left
	};
	const int vertexLen = sizeof(Vertex) / sizeof(float);
	int vertexByteSize = sizeof(vertices);
	const int vertexCount = vertexByteSize / vertexLen / sizeof(float);
	int usage = GL_STATIC_DRAW;

	// Create Indices
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 4,
		4, 5, 6,
		6, 7, 8,
		8, 9, 1,
		1, 2, 4,
		4, 1, 8,
		8, 6, 4
	};
	int indicesByteSize = sizeof(indices);
	const int indicesLen = indicesByteSize / sizeof(unsigned int);

	// Create VBO
	unsigned int vaoID = createVAO();
	createVBO(vertices, vertexByteSize, vertexLen, usage);
	createEBO(indices, indicesByteSize, usage);
	addVertexAttrib(0, 3, vertexLen, (void*)offsetof(Vertex, position));	// Position
	addVertexAttrib(1, 4, vertexLen, (void*)offsetof(Vertex, color));		// Color

	// Set clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Main loop
	while (!glfwWindowShouldClose(Window::nativeWindow)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle input
		Input::handleKeyInput();

		// Render
		useVAO(vaoID);
		shader->use();
		//glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glDrawElements(GL_TRIANGLES, indicesLen, GL_UNSIGNED_INT, 0);

		// Swap buffers & Handle window events
		glfwSwapBuffers(Window::nativeWindow);
		glfwPollEvents();
	}

	// Terminate
	delete shader;
	glfwDestroyWindow(Window::nativeWindow);
	glfwTerminate();
	return 0;
}