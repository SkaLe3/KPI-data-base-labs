#include "VieM/Core/Application.h"
#include "VieM/Core/EntryPoint.h"
#include "VieM/ImGui/DemoLayer.h"
#include "AppLayer.h"


VieM::Application* VieM::CreateApplication(int argc, char** argv)
{
	VieM::ApplicationSpecification spec;
	spec.Name = "MusiCatalog";
	spec.CustomTitleBar = true;
	spec.CenterWindow = true;
	spec.IconPath = "assets/images/logo.png";
	//spec.Width = 1600;
	//spec.Height = 900;

	VieM::Application* app = new VieM::Application(spec);

	std::shared_ptr<DemoLayer> demo = std::make_shared<DemoLayer>();
	std::shared_ptr<AppLayer> appLayer = std::make_shared<AppLayer>();
#if 0
	app->PushLayer(demo);
#else
	app->PushLayer(appLayer);
#endif
	

	return app;
}