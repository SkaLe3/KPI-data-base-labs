#pragma once
#include "WindowUI.h"
#include "MVC/Tables.h"

//temp
#include "VieM/Core/Log.h"
class WManagementDB : public IWindowUI
{
public:
	WManagementDB();
	virtual void OnRender() override;
	void Update(std::shared_ptr<TableData> data) { m_TableData = data; }
	void ControlPanel();
	void DrawTab(Table id);
	Table GetTabSelectionContext() { return m_TabSelectionContext; }

private:
	Table m_TabSelectionContext = Table::Genre;
	Table m_NewTabSelectionContext;

	int m_TableFlags;
	int m_ColumnFlags;

	int32_t m_GenerateRowsCount = 0;

	std::shared_ptr<TableData> m_TableData;

private:
};

