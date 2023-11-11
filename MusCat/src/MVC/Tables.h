#pragma once
#include <string>
#include <vector>

using TableData = std::vector<std::vector<std::string>>;

enum class Table : uint8_t
{
	Genre = 0,
	Label,
	Album,
	Song,
	Person,
	Artist,
	Artist_Person,
	Artist_Song
};

enum class AuxiliaryTable : uint8_t
{
	name = 0,
	surname,
	words
};

enum class ColumnType : uint8_t
{
	Serial = 0,
	Int,
	CharVar64,
	Date,
	Text,

};

struct Column
{
	std::string Name;
	ColumnType Type;
};

class TableSpecs
{
public:
	static std::string GetName(Table index) { return s_Names[(uint8_t)index]; }
	static std::vector<Column> GetColumns(Table index) { return s_Columns[(uint8_t)index]; }

private:
	static std::vector<std::string> s_Names;
	static std::vector<std::vector<Column>> s_Columns;
};

class AuxiliaryTableSpecs
{
public:
	static std::string GetName(AuxiliaryTable index) { return s_Names[(uint8_t)index]; }
	static std::vector<Column> GetColumns(AuxiliaryTable index) { return s_Columns[(uint8_t)index]; }

private:
	static std::vector<std::string> s_Names;
	static std::vector<std::vector<Column>> s_Columns;
};