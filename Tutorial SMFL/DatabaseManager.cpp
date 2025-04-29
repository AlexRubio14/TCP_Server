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
        std::cout << "Connection with DB DONE" << std::endl;
    }
    catch (sql::SQLException e)
    {
        std::cerr << "Could not connect to server because: " << e.what() << std::endl;
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

void DatabaseManager::CreateUser(std::string username, std::string password)
{
    try
    {
        std::string query = "INSERT INTO users (username, password_hash, salt) VALUES (?, ?, ?)";
        sql::PreparedStatement* stat = con->prepareStatement(query);

		std::string salt = GenerateSalt();

        std::string hashedPassword = GenerateHashedPassword(password);

        stat->setString(1, username);
        stat->setString(2, hashedPassword);
        stat->setString(3, salt);


        int affected_rows = stat->executeUpdate();

        if (affected_rows > 0)
        {
            std::cout << "User Created" << std::endl;
        }

        delete stat;
    }
    catch (sql::SQLException error)
    {
        std::cerr << "Error creating user because: " << error.what() << std::endl;
    }
}

void DatabaseManager::CheckUserLogin(std::string username, std::string password)
{

}

std::string DatabaseManager::GenerateHashedPassword(const std::string password)
{
    unsigned int hashRound = 12;

	std::string hash = bcrypt::generateHash(password, hashRound);

	std::cout << "Hash generated is : " << hash << std::endl;

    return hash;
}

std::string DatabaseManager::GenerateSalt()
{
    size_t lenght = 16;

    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string salt;

    std::srand(std::time(0));

    for (size_t i = 0; i < lenght; ++i) {
        int randomIndex = std::rand() % chars.size();  
        salt += chars[randomIndex];
    }

	std::cout << "Salt generated is : " << salt << std::endl;
    return salt;
}

