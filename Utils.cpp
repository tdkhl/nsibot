#include <string>
#include <iostream>
#include <sstream>
#include <dpp/dpp.h>
#include <vector>
#include <map>
#include "MyBot.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <xlocbuf>
#include <locale>
#include <codecvt>
using namespace std;



int Log(std::string msg)
{
	std::cout << msg << std::endl;
	return 0;
    
}

vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

void resetloggedusers()
{
    while (1)
    {
        loggedUsers.clear();
        Sleep(15 * 60000);
    }

}

std::string GetCurrentDirectoryaa()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

std::time_t parse_timestamp(string s)
{
    
    std::tm t{};
    std::istringstream ss(s);

    ss >> std::get_time(&t, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        throw std::runtime_error{ "Erreur lors du parsing du string en timestamp" };
    }
    std::time_t time_stamp = mktime(&t);
    std::cout << time_stamp;
    return time_stamp;
}

std::wstring StringToWString(const std::string& s)
{
    std::wstring wsTmp(s.begin(), s.end());

    wstring ws = wsTmp;

    return ws;
}


std::wstring utf8_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

std::string ws_to_s(const std::wstring& ws)
{
    std::string buffer(ws.begin(), ws.end());
    return buffer;
}

std::wstring string_to_wstring(const std::string& s)
{
    char* old = setlocale(LC_ALL, "");
    wchar_t buffer[500];
    mbstowcs(buffer, s.c_str(), sizeof(buffer)) != s.size();
    std::locale loc("");
    return buffer;
}


