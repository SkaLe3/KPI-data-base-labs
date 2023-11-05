#include "View.h"
#include "Controller.h"
#include <VieM/Core/Application.h>
#include <VieM/Core/Window.h>
#include <VieM/Core/Log.h>
#include <imgui/imgui.h>


View::View()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.CellPadding = ImVec2(10.0f, 8.0f);
	style.FrameRounding = 8.0f;

}

void View::OnRender()
{

	for (std::shared_ptr<IWindowUI> window : m_Windows)
	{
		if (window->IsNotHidden())
			window->OnRender();
	}

}

void View::AddListener(const std::shared_ptr<Controller>& controller)
{
	m_Controller = controller; 
}



void View::RemoveAllWindows()
{
	m_Windows.clear();
}

