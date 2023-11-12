#pragma once
#include <memory>
#include <string>
#include "View.h"
#include "MVC/Tables.h"
class Model;

class Controller : public std::enable_shared_from_this<Controller>
{
public:
	Controller(const std::shared_ptr<Model>& model, const std::shared_ptr<View>& view)
		: m_Model(model), m_View(view) { }

	~Controller() { m_View->RemoveAllWindows(); }
	void OnUpdate();
	void Run();
	void OnLogin(const std::string& dbname, const std::string& username, const std::string& password);

	void OnShowSelections();
	void OnShowManagement();

	void OnCreateRecord(Table id);
	void OnUpdateRecord(Table id);
	void OnDeleteRecord(Table id);
	double OnGenerateData(Table id, int32_t rowCount);

	void OnCreateRecordSubmit(Table id, std::vector<std::string> recordData);
	void OnUpdateRecordSubmit(Table id, std::vector<std::string> recordData, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData);
	void OnDeleteRecordSubmit(Table id, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData);

	void OnFetchData(Table id);
	bool OnFindRecord(Table id, std::vector<Column> columns, std::vector<std::string> data, IWindowUI* sender);

	// Selections
	double OnFirstSelection(int32_t age, int32_t songCount,int32_t genreId, std::string genre);
	double OnSecondSelection(const std::string& fromDate, const std::string& toDate);
	double OnThirdSelection(const std::string& fromDate, const std::string& toDate);



	std::string& GetErrorMessage() { return m_ErrorMessageText; }
public:
	void CreateWindows();
	template<class W>
	void ShowWindow()
	{
		m_View->ShowWindow<W>();
	}

	template<class W>
	void HideWindow()
	{
		m_View->HideWindow<W>();
	}
private:
	std::shared_ptr<Model> m_Model;
	std::shared_ptr<View> m_View;

	std::string m_ErrorMessageText;



};

