#include "Model.h"
#include <VieM/Core/Log.h>

Model::~Model()
{
	PQfinish(m_Connection);
}

bool Model::Connect(const std::string& username, const std::string& password, std::string& errorMessage)
{
	m_Connection = PQconnectdb(("dbname=postgres user=" + username + " password=" + password).c_str());

	if (PQstatus(m_Connection) == CONNECTION_BAD)
	{
		
		errorMessage = std::string("Connection to database failed:\n") +  PQerrorMessage(m_Connection);
		VM_ERROR(errorMessage);
		PQfinish(m_Connection);
		return false;
	}
	return true;
}
