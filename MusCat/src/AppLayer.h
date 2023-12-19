#pragma once
#include <Viem/Core/Layer.h>
#include <VieM/Events/Event.h>
#include <imgui/imgui.h>
#include <memory>

#include "Panels/SettingsPanel.h"
#include "Panels/FPS.h"


#include "MVC/Controller.h"
#include "MVC/ModelBase.h"
#include "MVC/View.h"

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
	FPS m_FPSPanel;

	std::shared_ptr<View> m_View;
	std::shared_ptr<ModelBase> m_Model;
	std::shared_ptr<Controller> m_Controller;
	

};

