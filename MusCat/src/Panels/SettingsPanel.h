#pragma once
// temp
#include "VieM/Core/Image.h"
#include <memory>

class SettingsPanel
{
public:
	SettingsPanel(){}
	void OnRender();
public:
	bool m_Enabled = false;

};