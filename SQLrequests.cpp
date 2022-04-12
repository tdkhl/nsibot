#include <iostream>
#include <memory>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"
#include "cppconn/prepared_statement.h"

using namespace sql::mysql;
using namespace std;

std::map<int, string> passwords;

int InsertMPWD(string id, string pwd)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection *con;
    sql::PreparedStatement *stmt;

    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "INSERT INTO nsi_users (id, master_password) VALUES (?,?)";
    stmt = con->prepareStatement(query);
    stmt->setString(1, id);
    stmt->setString(2, pwd);
    stmt->execute();

    delete stmt;
    delete con;
	return 0;
}
bool CheckLogin(string password, string id)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* stmt;
    sql::ResultSet* res;

    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "SELECT * FROM nsi_users WHERE id = ? AND master_password = ?";
    stmt = con->prepareStatement(query);
    stmt->setString(1, id);
    stmt->setString(2, password);
    res = stmt->executeQuery();

    while (res->next())
    {
        return true;
    }
    return false;
}
bool CheckIfRegistered(string id)
{
    

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* stmt;
    sql::ResultSet* res;

    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "SELECT * FROM nsi_users WHERE id = ?";
    stmt = con->prepareStatement(query);
    stmt->setString(1, id);
    res = stmt->executeQuery();

    while (res->next())
    {
        return true;
    }
    return false;
}
int InsertPWD(string id, string pwd, string identifier)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* stmt;

    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "INSERT INTO passwords (id, password, identifier) VALUES (?,?,?)";
    stmt = con->prepareStatement(query);
    stmt->setString(1, id);
    stmt->setString(2, pwd);
    stmt->setString(3, identifier);
    stmt->execute();

    delete stmt;
    delete con;
    return 0;
}
bool CheckIfExit(string id, string identifier)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* stmt;
    sql::ResultSet* res;

    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "SELECT * FROM passwords WHERE id = ? AND identifier = ?";
    stmt = con->prepareStatement(query);
    stmt->setString(1, id);
    stmt->setString(2, identifier);
    res = stmt->executeQuery();

    while (res->next())
    {
        return true;
    }
    return false;
}
int RenamePWD(string id, string identifier, string new_identifier)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* stmt;

    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "UPDATE passwords SET identifier = ? WHERE id = ? AND identifier = ?";
    stmt = con->prepareStatement(query);
    stmt->setString(1, new_identifier);
    stmt->setString(2, id);
    stmt->setString(3, identifier);
    stmt->execute();

    delete stmt;
    delete con;
    return 0;
}

int GetPasswords(string id)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* stmt;
    sql::ResultSet* res;
    int i = 0;
    driver = get_mysql_driver_instance();
    con = driver->connect("", "botd", "");
    con->setSchema("botd");

    string query = "SELECT * FROM passwords WHERE id = ?";
    stmt = con->prepareStatement(query);
    stmt->setString(1, id);
    res = stmt->executeQuery();

    while (res->next())
    {
        string pw = res->getString(2);
        string identifier = res->getString(3);
        passwords[i] = identifier + "_" + pw;
        i++;
    }
    return 0;
}
