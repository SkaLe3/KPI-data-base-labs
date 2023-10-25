#pragma once
#include <string>
#include "VieM/Core/Window.h"
struct GLFWwindow;
struct ImFont;
	
namespace VieM {

	namespace IGContext
	{

		void Init(GLFWwindow* window);
		void EnableDocking(Window& window);
		void RenderDockspace(Window& window);
		void BeginFrame();
		void EndFrame();

	}

}