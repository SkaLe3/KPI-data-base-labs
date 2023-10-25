#include "TilesPanel.h"
#include <imgui/imgui.h>
#include <thread>


void TilesPanel::DrawTile(bool* selected)
{

	
}

void TilesPanel::OnRender()
{
	ImGui::Begin("Frame rate");
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("%.1f FPS", io.Framerate);
	ImGui::End();

}
