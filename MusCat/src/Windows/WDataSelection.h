#pragma once
#include "WindowUI.h"
class WDataSelection : public IWindowUI
{
public:
	WDataSelection();
	virtual void OnRender() override;
	void SelectionPanel();
	int m_TableFlags;
	int m_ColumnFlags;
};

