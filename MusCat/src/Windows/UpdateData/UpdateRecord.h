#pragma once
#include "Windows/PopupWindows.h"
#include "Windows/WindowUI.h"

class WUpdateRecord : public PopupTableManagement, public IWindowUI
{
public:
	WUpdateRecord() : PopupTableManagement() {}
	virtual void OnRender() override;
	void Update(std::vector<std::string> data);
	void SetSelectionContext(Table id, const std::vector<Column>& keys);

private:
	std::vector<Column> m_Keys;
	std::vector<std::string> m_KeysData;
	bool m_RecordExist = false;

	std::vector<std::string> m_TextKeys;
	std::vector<int32_t> m_IntKeys;

};