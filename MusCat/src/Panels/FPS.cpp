#include "FPS.h"
#include <imgui/imgui.h>
#include <thread>



void FPS::OnRender()
{
	if (m_Enabled)
	{
		ImGui::Begin("Frame rate");
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("%.1f FPS", io.Framerate);
		ImGui::End();
	}
}
