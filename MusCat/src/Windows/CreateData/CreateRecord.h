#pragma once
#include "Windows/PopupWindows.h"
#include "Windows/WindowUI.h"

class WCreateRecord : public PopupTableManagement, public IWindowUI
{
public:
	WCreateRecord() : PopupTableManagement() {}
	virtual void OnRender() override;


private:


};