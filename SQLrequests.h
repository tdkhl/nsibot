#include <iostream>
#include <memory>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"
#include "cppconn/prepared_statement.h"

using namespace sql::mysql;
using namespace std;

#ifndef SQLREQUESTS_H
#define SQLREQUESTS_H
int InsertMPWD(string id, string pwd);
bool CheckIfRegistered(string id);
bool CheckLogin(string password, string id);
int InsertPWD(string id, string pwd, string identifier);
bool CheckIfExit(string id, string identifier);
int RenamePWD(string id, std::string identifier, std::string new_identifier);
extern std::map<int, string> passwords;
int GetPasswords(string id);
#endif