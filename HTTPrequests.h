#include <string>
extern std::map<std::string, std::string> Meteo;
#ifndef HTTPREQUESTS_H
#define HTTPREQUESTS_H

int GetMeteo(std::string location);

#endif