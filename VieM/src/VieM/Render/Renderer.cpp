#include "Renderer.h"

#include <Glad/glad.h>

namespace VieM {

	
	void Renderer::SetClearColor(float x, float y, float z, float w)
	{
		glClearColor(x, y, z, w);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		SetViewport(0, 0, width, height);
	}

}