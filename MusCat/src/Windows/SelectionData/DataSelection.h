#pragma once
#include "Windows/WindowUI.h"
class WDataSelection : public IWindowUI
{
public:
	WDataSelection();
	virtual void OnRender() override;
	void Update(std::shared_ptr<std::vector<std::vector<std::string>>> data);

	// Draws
	void SelectionPanel();
	void TablePanel();
	void FirstSelection();
	void SecondSelection();
	void ThirdSelection();


	// Settings
	void Setup(const std::vector<std::string>& genres) { m_Genres = genres; }
	void SetColumns(const std::vector<std::string>& columns) { m_Columns = columns; }
private:
	int m_TableFlags;
	int m_ColumnFlags;
private:

	std::shared_ptr<std::vector<std::vector<std::string>>> m_TableData;
	std::vector<std::string> m_Columns;

	std::vector<std::string> m_Genres;
	int32_t m_CurrentGenreIdx = 0;
	int32_t m_Age = 0;
	int32_t m_SongCount = 0;


	std::string m_FromDate2;
	std::string m_ToDate2;

	std::string m_FromDate3;
	std::string m_ToDate3;
};

