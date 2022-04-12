#include "HTTPRequest.hpp";
#include <iostream>
#include <sstream>
#include "HTTPrequests.h"
#include "nlohmann/json.hpp"
#include <xstring>
#include "Utils.h"




using json = nlohmann::json;
std::map<std::string, std::string> Meteo;

int GetMeteo(std::string location)
{
    try
    {
        
        std::string key = "6ce7b5c62e6b405280985118222101";
        std::ostringstream endpoint;


        endpoint << "http://api.weatherapi.com/v1/current.json?key=" << key << "&q=" << location << "&aqi=no" << "&lang=fr";
        http::Request request{endpoint.str()};
       
        
        const auto response = request.send("GET");
        
        std::string content = { response.body.begin(), response.body.end() };
   
    
            if (content.find("error") != std::string::npos)
            {
                Meteo["code"] = "Aucune location trouvée pour ce nom";
               

            }
            else
            {


                auto js = json::parse(content); 
                

                Meteo["code"] = "success";
                Meteo["name"] = js["location"]["name"]; 
                Meteo["region"] = js["location"]["region"]; 
                Meteo["pays"] = js["location"]["country"];
                Meteo["Ensoleillement"] = js["current"]["condition"]["text"];
                Meteo["region"] = js["location"]["region"];
                Meteo["pays"] = js["location"]["country"];
                Meteo["heure"] = js["location"]["localtime"];
                Meteo["update"] = js["current"]["last_updated"];
                Meteo["temp"] = to_string(js["current"]["temp_c"]);
                Meteo["daytime"] = to_string(js["current"]["is_day"]);
                Meteo["Ensoleillement"] = js["current"]["condition"]["text"];
                Meteo["icon"] = js["current"]["condition"]["icon"];
                Meteo["vent"] = to_string(js["current"]["wind_kph"]);
                Meteo["precip"] = to_string(js["current"]["precip_mm"]);
                Meteo["humid"] = to_string(js["current"]["humidity"]);
                Meteo["cloud"] = to_string(js["current"]["cloud"]);
                Meteo["ressenti"] = to_string(js["current"]["feelslike_c"]);
                Meteo["uv"] = to_string(js["current"]["uv"]);
                
               
                
            }

        
            



    }
    catch (const std::exception& e)
    {
        std::cerr << "Requête échouée, erreur " << e.what() << '\n';
    }
}