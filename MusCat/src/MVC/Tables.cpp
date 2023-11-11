#include "Tables.h"



std::vector<std::string> TableSpecs::s_Names = { 
	"Genre", "Label", "Album", "Song",
	"Person", "Artist", "Artist_Person", "Artist_Song" };

std::vector<Column> GenreColumns = { {"Genre_ID", ColumnType::Serial }, {"Name", ColumnType::CharVar64} };
std::vector<Column> LabelColumns = { {"Label_ID",ColumnType::Serial }, {"Name",ColumnType::CharVar64 }, {"Location",ColumnType::CharVar64 }, {"Founded_Date",ColumnType::Date } };
std::vector<Column> AlbumColumns = { {"Album_ID",ColumnType::Serial }, {"Title",ColumnType::CharVar64 } };
std::vector<Column> SongColumns = { {"Song_ID",ColumnType::Serial }, {"Title",ColumnType::CharVar64 }, {"Release_Date",ColumnType::Date }, {"Label_ID",ColumnType::Int }, {"Genre_ID",ColumnType::Int }, {"Album_ID",ColumnType::Int } };
std::vector<Column> PersonColumns = { {"Person_ID",ColumnType::Serial }, {"Name",ColumnType::CharVar64 }, {"Birth_Date",ColumnType::Date }, {"Death_Date",ColumnType::Date } };
std::vector<Column> ArtistColumns = { {"Artist_ID",ColumnType::Serial }, {"Title",ColumnType::CharVar64 }, {"Description",ColumnType::Text }, {"Founded_Date",ColumnType::Date}, {"Closed_Date",ColumnType::Date}, {"Label_ID",ColumnType::Int } };
std::vector<Column> ArtistPersonColumns = { {"Artist_ID", ColumnType::Int }, {"Person_ID", ColumnType::Int} };
std::vector<Column> AtistSongColumns = { {"Artist_ID", ColumnType::Int }, {"Song_ID", ColumnType::Int} };


std::vector<std::vector<Column>> TableSpecs::s_Columns =
{
	GenreColumns,
	LabelColumns,
	AlbumColumns,
	SongColumns,
	PersonColumns,
	ArtistColumns,
	ArtistPersonColumns,
	AtistSongColumns
};

std::vector<std::string> AuxiliaryTableSpecs::s_Names = {
	"names", "surnames", "words"
};

std::vector<Column> NamesColumns = { {"names_id", ColumnType::Serial }, {"data", ColumnType::CharVar64} };
std::vector<Column> SurnamesColumns = { {"surnames_id",ColumnType::Serial }, {"data", ColumnType::CharVar64 } };
std::vector<Column> WordsColumns = { {"words_id",ColumnType::Serial }, {"data", ColumnType::CharVar64 } };

std::vector<std::vector<Column>> AuxiliaryTableSpecs::s_Columns =
{
	NamesColumns,
	SurnamesColumns,
	WordsColumns
};