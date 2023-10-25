#pragma once


#include "KeyCodes.h"


namespace VieM {

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode);
		static bool IsMouseButtonDown(MouseButton button);

		static std::pair<float, float> GetMousePosition();

		static void SetCursorMode(CursorMode mode);
	};

}