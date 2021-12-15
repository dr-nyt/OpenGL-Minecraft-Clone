#include "core.h"
#include "window/window.h"
#include "window/input.h"

using namespace MinecraftClone;

void handleKeyInput();

int main() {
	const int windowWidth = 1080;
	const int windowHeight = 720;
	const bool fullScreenMode = false;

	// Create Window
	const bool success = Window::createWindow(windowWidth, windowHeight, "Minecraft Clone", fullScreenMode);
	if (!success) return -1;

	// Main loop
	while (!glfwWindowShouldClose(Window::nativeWindow)) {
		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle input
		handleKeyInput();

		// Swap buffers & Handle window events
		glfwSwapBuffers(Window::nativeWindow);
		glfwPollEvents();
	}

	// Terminate
	glfwDestroyWindow(Window::nativeWindow);
	glfwTerminate();
	return 0;
}

void handleKeyInput() {
	if (Input::isKeyDown(GLFW_KEY_ESCAPE)) {
		Window::close();
	}
}