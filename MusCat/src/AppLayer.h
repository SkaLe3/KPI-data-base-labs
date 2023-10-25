#pragma once
#include <Viem/Core/Layer.h>
#include <VieM/Events/Event.h>
#include <imgui/imgui.h>

#include "Panels/SettingsPanel.h"
#include "Panels/TilesPanel.h"

class AppLayer : public VieM::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnUIRender() override;

	virtual void OnEvent(VieM::Event& e) override {}
private:

	ImGuiIO& io = ImGui::GetIO();
	
	SettingsPanel m_SettignsPanel;
	TilesPanel m_TilesPanel;

};