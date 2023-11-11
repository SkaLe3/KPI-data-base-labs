#pragma once
#include "Windows/WindowUI.h"
class WDataSelection : public IWindowUI
{
public:
	WDataSelection();
	virtual void OnRender() override;
	void SelectionPanel();
	void Setup(const std::vector<std::string>& genres) { m_Genres = genres; }
private:
	int m_TableFlags;
	int m_ColumnFlags;
private:
	std::vector<std::string> m_Genres;
};

