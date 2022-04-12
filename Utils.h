#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
#ifndef UTILS_H
#define UTILS_H
int Log(string msg);
vector<string> split(string s, string delimiter);
void resetloggedusers();
std::string GetCurrentDirectoryaa();
std::time_t parse_timestamp(string s);
std::wstring StringToWString(const std::string& s);
std::wstring utf8_to_wstring(const std::string& str);
std::wstring string_to_wstring(const std::string& s);
std::string ws_to_s(const std::wstring& ws);
#endif