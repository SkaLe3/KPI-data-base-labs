#include "Model.h"
#include <VieM/Core/Log.h>

Model::~Model()
{
	Finish();
}

void Model::Finish()
{
	PQfinish(m_Connection);
}

bool Model::Connect(const std::string& username, const std::string& password, std::string& errorMessage)
{
	m_Connection = PQconnectdb(("dbname=MusicCatalog user=" + username + " password=" + password).c_str());

	if (PQstatus(m_Connection) == CONNECTION_BAD)
	{
		
		errorMessage = std::string("Connection to database failed:\n") +  PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		Finish();
		return false;
	}
	return true;
}

bool Model::CreateTables(std::string& errorMessage)
{
	errorMessage = "";
	std::string error;

	error = CreateTableGenre();
	if (!error.empty())
		errorMessage = error;

	error = CreateTableLabel();
	if (!error.empty())
		errorMessage = error;

	error = CreateTableAlbum();
	if (!error.empty())
		errorMessage = error;

	error = CreateTableSong();
	if (!error.empty())
		errorMessage = error;

	error = CreateTablePerson();
	if (!error.empty())
		errorMessage = error;

	error = CreateTableArtist();
	if (!error.empty())
		errorMessage = error;

	error = CreateTableArtist_Person();
	if (!error.empty())
		errorMessage = error;

	error = CreateTableArtist_Song();
	if (!error.empty())
		errorMessage = error;

	return errorMessage.empty();
}

std::string Model::CreateTableGenre()
{
	const char* createTableSQL = 
		"CREATE TABLE IF NOT EXISTS genre ("
		"genre_id serial NOT NULL,"
		"name character varying(64) NOT NULL,"
		"CONSTRAINT genre_id_pk PRIMARY KEY (genre_id));";

	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK , "genre");
}

std::string Model::CreateTableLabel()
{
	const char* createTableSQL = 
		"CREATE TABLE IF NOT EXISTS label ("
		"label_id serial NOT NULL,"
		"name character varying(64) NOT NULL,"
		"location character varying(64) NOT NULL,"
		"founded_date date NOT NULL,"
		"CONSTRAINT label_id_pk PRIMARY KEY (label_id));";

	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "label");
}

std::string Model::CreateTableSong()
{
	const char* createTableSQL = 
		"CREATE TABLE IF NOT EXISTS song ("
		"song_id serial NOT NULL,"
		"title character varying(64) NOT NULL,"
		"release_date date NOT NULL,"
		"label_id integer,"
		"genre_id integer NOT NULL,"
		"album_id integer,"
		"CONSTRAINT song_id_pk PRIMARY KEY (song_id),"
		"CONSTRAINT song_album_id_fk FOREIGN KEY (album_id)"
			"REFERENCES public.album (album_id),"
		"CONSTRAINT song_genre_id_fk FOREIGN KEY (genre_id)"
			"REFERENCES public.genre (genre_id),"
		"CONSTRAINT song_label_id_fk FOREIGN KEY (label_id)"
			"REFERENCES public.label (label_id))";

	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "song");
}


std::string Model::CreateTableAlbum()
{
	const char* createTableSQL =
		"CREATE TABLE IF NOT EXISTS album ("
		"album_id serial NOT NULL,"
		"title character varying(64) NOT NULL,"
		"CONSTRAINT album_id_pk PRIMARY KEY (album_id))";

	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "album");
}

std::string Model::CreateTablePerson()
{
	const char* createTableSQL =
		"CREATE TABLE IF NOT EXISTS person ("
		"person_id serial NOT NULL,"
		"name character varying(64) NOT NULL,"
		"birth_date date NOT NULL,"
		"death_date date,"
		"CONSTRAINT person_id_pk PRIMARY KEY (person_id))";

	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "person");
}

std::string Model::CreateTableArtist()
{
	const char* createTableSQL =
		"CREATE TABLE IF NOT EXISTS artist ("
		"artist_id serial NOT NULL,"
		"title character varying(64) NOT NULL,"
		"description text,"
		"founded_date date NOT NULL,"
		"closed_date date,"
		"label_id integer,"
		"CONSTRAINT artist_id_pk PRIMARY KEY (artist_id),"
		"CONSTRAINT artist_label_id_fk FOREIGN KEY (label_id)"
			"REFERENCES public.label (label_id))";

	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "artist");
}

std::string Model::CreateTableArtist_Person()
{
	const char* createTableSQL =
		"CREATE TABLE IF NOT EXISTS artist_person ("
		"artist_id integer NOT NULL,"
		"person_id integer NOT NULL,"
		"CONSTRAINT artist_person_pk PRIMARY KEY (artist_id, person_id),"
		"CONSTRAINT artist_person_artist_id_fk FOREIGN KEY (artist_id)"
			"REFERENCES public.artist (artist_id),"
		"CONSTRAINT artist_person_person_id_fk FOREIGN KEY (person_id)"
			"REFERENCES public.person (person_id))";


	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "artist_person");
}

std::string Model::CreateTableArtist_Song()
{
	const char* createTableSQL =
		"CREATE TABLE IF NOT EXISTS artist_song ("
		"artist_id integer NOT NULL,"
		"song_id integer NOT NULL,"
		"CONSTRAINT artist_song_pk PRIMARY KEY (artist_id, song_id),"
		"CONSTRAINT artist_song_artist_id_fk FOREIGN KEY (artist_id)"
			"REFERENCES public.artist (artist_id),"
		"CONSTRAINT artist_song_song_id_fk FOREIGN KEY (song_id)"
			"REFERENCES public.song (song_id))";


	PGresult* res = PQexec(m_Connection, createTableSQL);

	return CheckResult(res, PGRES_COMMAND_OK, "artist_song");
}

std::string Model::CheckResult(PGresult* res, ExecStatusType status, const std::string& text)
{
	if (PQresultStatus(res) != status)
	{
		std::string errorMessage = text + std::string(" - Query execution failed:\n") + PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQclear(res);
		Finish();
		return errorMessage;
	}
	PQclear(res);
	return std::string();
}


