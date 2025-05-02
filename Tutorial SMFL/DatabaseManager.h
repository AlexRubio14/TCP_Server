#pragma once
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#define SERVER "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "enti"
#define DATABASE "parchis_tcp"

#define DB_MANAGER DatabaseManager::Instance()

class DatabaseManager
{
private:

	sql::Driver* driver;
	sql::Connection* con;

	DatabaseManager() = default;
	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager& operator=(const DatabaseManager&) = delete;

public:
	static DatabaseManager& Instance()
	{
		static DatabaseManager instance;
		return instance;
	}
	void ConnectDb();
	void DisconnectDb();
	bool CreateUser(const std::string username, const std::string password);
	void CheckUserLogin(const std::string username, const std::string password);

	std::string GenerateHashedPassword(const std::string password);
};

