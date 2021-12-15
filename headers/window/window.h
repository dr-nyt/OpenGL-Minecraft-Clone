#pragma once
#include "core.h"

namespace MinecraftClone
{
	namespace Window
	{
		extern GLFWwindow* nativeWindow;
		extern int windowWidth;
		extern int windowHeight;

		void addWindowCallbacks();
		void close();
		void windowSizeCallback(GLFWwindow* window, int width, int height);
		bool createWindow(
			int width,
			int height,
			const char* title,
			bool fullScreenMode
		);
	}
}