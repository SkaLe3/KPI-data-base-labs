#pragma once
#include <cstdint>
namespace VieM {

	class Renderer
	{
	public:
		static void SetClearColor(float x, float y, float z, float w);
		static void Clear();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void OnWindowResized(uint32_t width, uint32_t height);
	};
}