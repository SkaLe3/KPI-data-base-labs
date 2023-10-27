#pragma once
#include <libpq-fe.h>
#include <string>
class Model
{
public:
	~Model();
	bool Connect(const std::string& username, const std::string& password, std::string& errorMessage);

private:
	PGconn* m_Connection;
};

