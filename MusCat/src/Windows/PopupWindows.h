#pragma once
#include "WindowUI.h"
#include <memory>
#include <regex>
#include "MVC/Tables.h"


class PopupTableManagement
{
public:
	PopupTableManagement();
	void SetSelectionContext(Table id);
	int32_t CenterNextWindow(bool flag, int32_t width);
	void DrawEditRecord(int32_t width);
	void DrawFindRecord(int32_t width, std::vector<Column> keyColumns, std::vector<std::string>& textKeysData, std::vector<int32_t>& intKeysData);
protected:
	Table m_SelectionContext;

	int m_TableFlags;
	int m_ColumnFlags;

	std::vector<std::string> m_TextBuffer;
	std::vector<int32_t> m_IntBuffer;
};

