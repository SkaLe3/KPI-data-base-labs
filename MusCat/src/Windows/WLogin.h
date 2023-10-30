#pragma once
#include "WindowUI.h"
class WLogin : public IWindowUI
{
public:
	virtual void OnRender() override;
private:
	char m_Username[32] = "";
	char m_Password[32] = "";
};

