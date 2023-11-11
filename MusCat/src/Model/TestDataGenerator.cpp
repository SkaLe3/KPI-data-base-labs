#include "TestDataGenerator.h"
#include "VieM/Core/Log.h"
#include <random>

// if rowcount == 0 - delete all in table
bool TestDataGenerator::GenerateGenres(std::vector<std::string>& genres, std::string& errorMessage)
{
	if (!DeleteData(Table::Genre, errorMessage))
		return false;


	for (const std::string& value : genres)
	{
		std::string query = "INSERT INTO genre (name) VALUES ('" + value + "')";

		PGresult* result = PQexec(m_Connection, query.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			errorMessage = std::string("Failed to insert data into the table genre: \n") + PQerrorMessage(m_Connection);
			VM_ERROR(errorMessage);
			PQclear(result);
			PQfinish(m_Connection);
			return false;
		}

		PQclear(result);

	}
	errorMessage = "";
	return true;
}

bool TestDataGenerator::GenerateLabels(std::vector<std::string>& labels, std::vector<std::string>& locations, std::string& errorMessage)
{
	std::random_device rd;
	uint32_t seed = rd();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<int> distribution(0, locations.size()-1);

	if (!DeleteData(Table::Label, errorMessage))
		return false;

	std::vector<std::string> labelLocations;
	for (const std::string& label : labels)
	{
		labelLocations.push_back(locations[distribution(gen)]);
	}

	std::string query = "INSERT INTO label (name, location, founded_date) ";
	query += "SELECT "
		"titles.title, "
		"titles.loc, "
		"date_trunc('day', current_date - (random() * 365* 40 +365*5 || 'days')::interval) AS date "
		"FROM "
		"(VALUES ";
	for (int32_t i = 0; i < labels.size(); ++i)
	{
		if (i > 0)
			query += ",";

		query += "('" + labels[i] + "', '" + labelLocations[i] + "')";
	}
	query += ") AS titles(title, loc)";


	PGresult* result = PQexec(m_Connection, query.c_str());

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table label: \nYou cannot regenerate labels that already in use") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		PQfinish(m_Connection);
		return false;
	}

	PQclear(result);

	return true;
}

bool TestDataGenerator::GenerateAlbums(int32_t rowCount, std::string& errorMessage)
{
	if (!DeleteData(Table::Album, errorMessage))
		return false;

	std::string functionQuery =
		// Function to make sequence like 111 22 3333 4 55 66, where amount of number repetition is random in range 1-5 and max number is specified for first column
		// and pick random id from auxiliary table words for second column
		"CREATE OR REPLACE FUNCTION generate_random_sequence(max_number INT) "
		"RETURNS TABLE(amount_of_words INT, word_id INT) AS $$ "
		"BEGIN "
			"FOR i IN 1..max_number LOOP "
				"FOR j IN 1..floor(random() * 5) + 1 LOOP "
					"amount_of_words := i;"
					"word_id := floor(random() * (select count(*) from words)::int + 1); "
					"RETURN NEXT; "
				"END LOOP; "
			"END LOOP; "
		"END; "
		"$$ LANGUAGE plpgsql; ";


	std::string insertQuery =

		"WITH tab AS( "
		"SELECT * FROM generate_random_sequence($1)), "
		// Join tables to replace word id with actual word
		"new_words AS( "
		"SELECT tab.amount_of_words, words.data "
		"FROM tab "
			"INNER JOIN words on tab.word_id = words.words_id "
		") "

		// Concatenate words with same number in first column to make title
		", only_titles AS( "
		"SELECT title FROM( "
		"SELECT amount_of_words as title_id, string_agg(data, ' ' order by random()) AS title "
		"FROM new_words "
		"GROUP BY amount_of_words) AS new_titles) "


		"INSERT INTO album (title) "
		"SELECT* FROM only_titles ";

	// Prepare the statement and bind the parameters.
	std::string temp = std::to_string(rowCount);
	const char* amountOfData[1] = {temp.c_str()};
	
	PGresult* result = PQexec(m_Connection, functionQuery.c_str());
	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to create function: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	result = PQexecParams(m_Connection, insertQuery.c_str(), 1,nullptr,  amountOfData, nullptr, nullptr, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table album: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;
}

bool TestDataGenerator::GenerateSong(int32_t rowCount, std::string& errorMessage)
{
	if (!DeleteData(Table::Song, errorMessage))
		return false;

	std::string functionQuery =
		// Function to make sequence like 111 22 3333 4 55 66, where amount of number repetition is random in range 1-5 and max number is specified for first column
		// and pick random id from auxiliary table words for second column
		"CREATE OR REPLACE FUNCTION generate_random_sequence(max_number INT) "
		"RETURNS TABLE(amount_of_words INT, word_id INT) AS $$ "
		"BEGIN "
		"	FOR i IN 1..max_number LOOP "
		"		FOR j IN 1..floor(random() * 5) + 1 LOOP "
		"			amount_of_words := i;"
		"			word_id := floor(random() * (select count(*) from words)::int + 1); "
		"			RETURN NEXT; "
		"		END LOOP; "
		"	END LOOP; "
		"END; "
		"$$ LANGUAGE plpgsql; ";

	std::string insertQuery =
		"WITH tab AS( " 
		"	SELECT * FROM generate_random_sequence($1)), "

		//--Join tables to replace word id with actual word
		"new_words AS( "
		"SELECT tab.amount_of_words, words.data "
		"	FROM tab "
		"	INNER JOIN words ON tab.word_id = words.words_id "
		") "
		//--Concatenate words into titles
		", only_titles AS( "
		"SELECT amount_of_words AS title_id, string_agg(data, ' ' order by random()) AS title "
		"	FROM new_words "
		"	GROUP BY amount_of_words) "
		
		//--Label Table with idand random number in range 1 - count label
		", pick_label AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM label) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "
		//--Label Table with numbered labels from 1 to count
		", label_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY label.label_id) AS number, "
		"	label.label_id "
		"	FROM label "
		") "
		//--Label Table with idand label_id
		", picked_label AS( "
		"	SELECT pick_label.number AS id, label_number.label_id "
		"	FROM pick_label "
		"	LEFT JOIN label_number ON pick_label.rnd = label_number.number "
		") "
		
		//--Label Table with nulls
		", ready_label AS( "
		"	SELECT id, "
		"	CASE "
		"	WHEN(id + label_id) % 2 = 0 THEN NULL "
		"	ELSE label_id "
		"	END AS label_id "
		"	FROM picked_label "
		") "
		
		//--Genre Table with idand random number in range 1 - count genre 
		", pick_genre AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM genre) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "
		//--Genre Table with numbered genres from 1 to count
		", genre_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY genre.genre_id) AS number, "
		"	genre.genre_id "
		"	FROM genre "
		") "
		//--Genre Table with idand genre_id
		", ready_genre AS( "
		"	SELECT pick_genre.number AS id, genre_number.genre_id "
		"	FROM pick_genre "
		"	INNER JOIN genre_number ON pick_genre.rnd = genre_number.number "
		") "
		
		//--Album Table with idand random number in range 1 - count album
		", pick_album AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM album) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "
		//--Album Table with numbered albums from 1 to count
		", album_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY album.album_id) AS number, "
		"	album.album_id "
		"	FROM "
		"	album "
		") "
		//--Album Table with idand album_id
		", picked_album AS( "
		"	SELECT pick_album.number AS id, album_number.album_id "
		"	FROM pick_album "
		"	LEFT JOIN album_number ON pick_album.rnd = album_number.number "
		") "
		
		//--Album Table with nulls
		", ready_album AS( "
		"	SELECT id, "
		"	CASE "
		"	WHEN(id + album_id) % 5 = 0 THEN NULL "
		"	ELSE album_id "
		"	END AS album_id "
		"	FROM picked_album "
		") "
		
		", results AS "
		"( "
		"	SELECT "
		"	only_titles.title, "
		"	date_trunc('day', current_date - (random() * 365 * 40 + 10 || 'days')::interval)::date AS date, "
		"	ready_label.label_id, "
		"	ready_genre.genre_id, "
		"	ready_album.album_id "
		
		"	FROM only_titles "
		"	INNER JOIN ready_label ON only_titles.title_id = ready_label.id "
		"	INNER JOIN ready_genre ON only_titles.title_id = ready_genre.id "
		"	INNER JOIN ready_album ON only_titles.title_id = ready_album.id) "
		
		"INSERT INTO song (title, release_date, label_id, genre_id, album_id) "
		"SELECT * FROM results ";

	std::string temp = std::to_string(rowCount);
	const char* amountOfData[1] = { temp.c_str() };

	PGresult* result = PQexec(m_Connection, functionQuery.c_str());
	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to create function: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	result = PQexecParams(m_Connection, insertQuery.c_str(), 1, nullptr, amountOfData, nullptr, nullptr, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table song: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;
}

bool TestDataGenerator::GeneratePerson(int32_t rowCount, std::string& errorMessage)
{
	if (!DeleteData(Table::Person, errorMessage))
		return false;

	std::string insertQuery =
		"WITH rnd_numbers AS( "
		"SELECT "
		"floor(random() * (SELECT count(*) FROM names) + 1) as rnd_name, "
		"floor(random() * (SELECT count(*) FROM surnames) + 1) as rnd_surname "
		"FROM generate_series(1, $1) "
		") "

		", results AS( "
		"	SELECT "
		"	names.data || ' ' || surnames.data, "
		"	date_trunc('day', current_date - (random() * 365 * 40 + 365 * 18 || 'days')::interval)::date AS date "
		"	FROM rnd_numbers "
		"	INNER JOIN names ON rnd_name = names.names_id "
		"	INNER JOIN surnames ON rnd_surname = surnames.surnames_id "
		") "
		"INSERT INTO person (name, birth_date) "
		"SELECT* FROM results ";

	std::string temp = std::to_string(rowCount);
	const char* amountOfData[1] = { temp.c_str() };


	PGresult* result = PQexecParams(m_Connection, insertQuery.c_str(), 1, nullptr, amountOfData, nullptr, nullptr, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table person: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;
}

bool TestDataGenerator::GenerateArtist(int32_t rowCount, std::string& errorMessage)
{
	if (!DeleteData(Table::Artist, errorMessage))
		return false;

	std::string functionQuery =
		"CREATE OR REPLACE FUNCTION generate_random_sequence(max_number INT) "
		"RETURNS TABLE(amount_of_words INT, word_id INT) AS $$ "
		"BEGIN "
		"	FOR i IN 1..max_number LOOP "
		"		FOR j IN 1..floor(random() * 5) + 1 LOOP "
		"			amount_of_words := i; "
		"			word_id := floor(random() * (select count(*) from words)::int + 1); "
		"			RETURN NEXT; "
		"		END LOOP; "
		"	END LOOP; "
		"END; "
		"$$ LANGUAGE plpgsql; ";

	std::string insertQuery =
		"WITH tab AS( "
		"	SELECT * FROM generate_random_sequence($1)), "

		//--Join tables to replace word id with actual word
		"new_words AS( "
		"	SELECT tab.amount_of_words, words.data "
		"	FROM tab "
		"	INNER JOIN words ON tab.word_id = words.words_id "
		") "
		//--Concatenate words into titles
		", only_titles AS( "
		"	SELECT amount_of_words AS title_id, string_agg(data, ' ' order by random()) AS title "
		"	FROM new_words "
		"	GROUP BY amount_of_words) "

		//--Label Table with idand random number in range 1 - count label
		", pick_label AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM label) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "
		//--Label Table with numbered labels from 1 to count
		", label_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY label.label_id) AS number, "
		"	label.label_id "
		"	FROM "
		"	label "
		") "
		//--Label Table with idand label_id
		", picked_label AS( "
		"	SELECT pick_label.number AS id, label_number.label_id "
		"	FROM pick_label "
		"	LEFT JOIN label_number ON pick_label.rnd = label_number.number "
		") "

		//--Label Table with nulls
		", ready_label AS( "
		"	SELECT id, "
		"	CASE "
		"		WHEN(id + label_id) % 2 = 0 THEN NULL "
		"		ELSE label_id "
		"		END AS label_id "
		"	FROM picked_label "
		") "

		", results AS( "
		"	SELECT "
		"		only_titles.title, "
		"		NULL AS description, "
		"		date_trunc('day', current_date - (random() * 365 * 40 + 10 || 'days')::interval)::date AS founded_date, "
		"		NULL::date AS closed_date, "
		"		ready_label.label_id "
		"		FROM only_titles "
		"		INNER JOIN ready_label ON only_titles.title_id = ready_label.id "
		") "
		"INSERT INTO artist (title, description, founded_date, closed_date, label_id) "
		"SELECT * FROM results ";

	std::string temp = std::to_string(rowCount);
	const char* amountOfData[1] = { temp.c_str() };

	PGresult* result = PQexec(m_Connection, functionQuery.c_str());
	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to create function: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	result = PQexecParams(m_Connection, insertQuery.c_str(), 1, nullptr, amountOfData, nullptr, nullptr, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table artist: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;

}

bool TestDataGenerator::GenerateArtist_Person(int32_t rowCount, std::string& errorMessage)
{
	if (!DeleteData(Table::Artist_Person, errorMessage))
		return false;

	std::string insertQuery =
		//--Artist Table with numbered artists from 1 to count
		"WITH artist_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY artist.artist_id) AS number, "
		"	artist.artist_id "
		"	FROM "
		"	artist "
		") "
		//--Artist Table with id and random number in range 1 - count artist
		", pick_artist AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM artist) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "

		//--Artist Table with idand artist_id
		", ready_artist AS( "
		"	SELECT pick_artist.number AS id, artist_number.artist_id "
		"	FROM pick_artist "
		"	LEFT JOIN artist_number ON pick_artist.rnd = artist_number.number "
		"	ORDER BY id "
		") "

		//--Person Table with numbered persons from 1 to count
		", person_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY person.person_id) AS number, "
		"	person.person_id "
		"	FROM "
		"	person "
		") "
		//--Person Table with idand random number in range 1 - count person
		", pick_person AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM person) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "

		//--Person Table with idand person_id
		", ready_person AS( "
		"	SELECT pick_person.number AS id, person_number.person_id "
		"	FROM pick_person "
		"	LEFT JOIN person_number ON pick_person.rnd = person_number.number "
		"	ORDER BY id "
		") "

		", results AS( "
		"	SELECT ready_artist.artist_id, ready_person.person_id "
		"	FROM ready_artist "
		"	INNER JOIN ready_person ON ready_artist.id = ready_person.id "
		"	ORDER BY artist_id, person_id "
		") "
		"INSERT INTO artist_person (artist_id, person_id) "
		"SELECT DISTINCT * FROM results";

	std::string temp = std::to_string(rowCount);
	const char* amountOfData[1] = { temp.c_str() };


	PGresult* result = PQexecParams(m_Connection, insertQuery.c_str(), 1, nullptr, amountOfData, nullptr, nullptr, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table artist_person: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;
}

bool TestDataGenerator::GenerateArtist_Song(int32_t rowCount, std::string& errorMessage)
{
	if (!DeleteData(Table::Artist_Song, errorMessage))
		return false;

	std::string insertQuery =
		//--Artist Table with numbered artists from 1 to count
		"WITH artist_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY artist.artist_id) AS number, "
		"	artist.artist_id "
		"	FROM "
		"	artist "
		") "
		//--Artist Table with id and random number in range 1 - count artist
		", pick_artist AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM artist) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "

		//--Artist Table with idand artist_id
		", ready_artist AS( "
		"	SELECT pick_artist.number AS id, artist_number.artist_id "
		"	FROM pick_artist "
		"	LEFT JOIN artist_number ON pick_artist.rnd = artist_number.number "
		"	ORDER BY id "
		") "

		//--Song Table with numbered songs from 1 to count
		", song_number AS( "
		"	SELECT "
		"	ROW_NUMBER() OVER(ORDER BY song.song_id) AS number, "
		"	song.song_id "
		"	FROM "
		"	song "
		") "
		//--Song Table with id and random number in range 1 - count song
		", pick_song AS( "
		"	SELECT  number, floor(random() * (SELECT count(*) FROM song) + 1) as rnd "
		"	FROM generate_series(1, $1) as number "
		") "

		//--Song Table with id and song_id
		", ready_song AS( "
		"	SELECT pick_song.number AS id, song_number.song_id "
		"	FROM pick_song "
		"	LEFT JOIN song_number ON pick_song.rnd = song_number.number "
		"	ORDER BY id "
		") "

		", results AS( "
		"	SELECT ready_artist.artist_id, ready_song.song_id "
		"	FROM ready_artist "
		"	INNER JOIN ready_song ON ready_artist.id = ready_song.id "
		"	ORDER BY artist_id, song_id "
		") "
		"INSERT INTO artist_song (artist_id, song_id) "
		"SELECT DISTINCT* FROM results";

	std::string temp = std::to_string(rowCount);
	const char* amountOfData[1] = { temp.c_str() };


	PGresult* result = PQexecParams(m_Connection, insertQuery.c_str(), 1, nullptr, amountOfData, nullptr, nullptr, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to insert data into the table artist_song: \n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;
}

bool TestDataGenerator::DeleteData(Table table, std::string& errorMessage)
{
	std::string tableName = TableSpecs::GetName(table);

	std::string deleteQuery = "DELETE FROM " + tableName;

	PGresult* result = PQexec(m_Connection, deleteQuery.c_str());
	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		errorMessage = std::string("Failed to clear data from the table ") + tableName +  " : \n" 
			"You cannot regenerate data that already in use.\nFirst delete child data\n"  + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}
	PQclear(result);

	if (TableSpecs::GetColumns(table)[0].Type != ColumnType::Serial)
		return true;

	std::string tableNameCopy = tableName;
	std::transform(tableNameCopy.begin(), tableNameCopy.end(), tableNameCopy.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);

	std::string columnCopy = TableSpecs::GetColumns(table)[0].Name;
	std::transform(columnCopy.begin(), columnCopy.end(), columnCopy.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);

	std::string resetQuery = "SELECT setval('" + tableNameCopy + "_" + columnCopy + "_seq',1 ,false );";

	result = PQexec(m_Connection, resetQuery.c_str());

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		errorMessage = std::string("Failed to reset serial from the table ") + tableName + " : \n" + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(result);
		return false;
	}

	PQclear(result);
	return true;
}

void TestDataGenerator::SetConnection(PGconn* connection)
{
	m_Connection = connection;
}
