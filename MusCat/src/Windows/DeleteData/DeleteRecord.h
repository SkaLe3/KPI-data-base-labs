#pragma once
#include "Windows/WindowUI.h"
#include "Windows/PopupWindows.h"
#include "MVC/Tables.h"

class WDeleteRecord : public PopupTableManagement, public IWindowUI
{
public:
	virtual void OnRender() override;
	void Update(std::vector<std::string> data);
	void SetSelectionContext(Table id, const std::vector<Column>& keys);
	void DrawRecordData();
private:
	std::vector<Column> m_Keys;
	std::vector<std::string> m_KeysData;
	bool m_RecordExist = false;

	std::vector<std::string> m_TextKeys;
	std::vector<int32_t> m_IntKeys;
};