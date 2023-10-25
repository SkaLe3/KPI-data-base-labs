#pragma once
#include "Application.h"

extern VieM::Application* VieM::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace VieM {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			VieM::Application* app = VieM::CreateApplication(argc, argv);
			app->Run();
			delete app;
		}

		return 0;
	}
}



#ifdef VM_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return VieM::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return VieM::Main(argc, argv);
}

#endif // VM_DIST

