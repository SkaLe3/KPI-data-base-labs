#pragma once
#include "VieM/Events/Event.h"

namespace VieM {

	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(float ts) {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Event& e) {}
	};
}