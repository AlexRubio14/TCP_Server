#include "DatabaseManager.h"
#include "bcrypt/include/bcrypt.h"
#include <cassert>

void DatabaseManager::ConnectDb()
{
    try
    {
        driver = get_driver_instance();
        con = driver->connect(SERVER, USERNAME, PASSWORD);
        con->setSchema(DATABASE);
        std::cout << "Connection with Db DONE" << std::endl;
    }
    catch (sql::SQLException e)
    {
        std::cerr << "Connection with Db failed because: " << e.what() << std::endl;
    }
}

void DatabaseManager::DisconnectDb()
{
    con->close();

    if (con->isClosed())
    {
        std::cout << "Connection close" << std::endl;
        delete con;
    }
}

bool DatabaseManager::CreateUser(const std::string username, const std::string password)
{
    try
    {
        std::string query = "INSERT INTO users (username, password) VALUES (?, ?)";
        sql::PreparedStatement* stat = con->prepareStatement(query);

        std::string hashedPassword = GenerateHashedPassword(password);

        stat->setString(1, username);
        stat->setString(2, hashedPassword);


        int affected_rows = stat->executeUpdate();

        if (affected_rows > 0)
        {
            std::cout << "User Created" << std::endl;
            return true;
        }

        delete stat;

        return false;
    }
    catch (sql::SQLException error)
    {
        std::cerr << "Error creating user because: " << error.what() << std::endl;
    }
}

void DatabaseManager::CheckUserLogin(const std::string username, const std::string password)
{

}

std::string DatabaseManager::GenerateHashedPassword(const std::string password)
{
    unsigned int hashRound = 12;

	std::string hash = bcrypt::generateHash(password, hashRound);

    return hash;
}

