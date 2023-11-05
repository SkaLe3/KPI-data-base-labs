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
protected:
	Table m_SelectionContext;

	int m_TableFlags;
	int m_ColumnFlags;

	std::vector<std::string> m_TextBuffer;
	std::vector<int32_t> m_IntBuffer;
};


class WAddRecord :  public PopupTableManagement, public IWindowUI
{
public:
	WAddRecord() : PopupTableManagement() {}
	virtual void OnRender() override;


private:


};

class WEditRecord : public PopupTableManagement, public IWindowUI
{
public:
	WEditRecord() : PopupTableManagement() {}
	virtual void OnRender() override;
	 void SetSelectionContext(Table id, const std::vector<Column>& keys);
	
	std::vector<Column> m_Keys;
	bool m_RowExist = false;

	std::vector<std::string> m_TextKeys;
	std::vector<int32_t> m_IntKeys;

};

class WDeleteRecord : public IWindowUI
{
public:
	virtual void OnRender() override;

};