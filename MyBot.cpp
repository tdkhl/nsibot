#include <dpp/dpp.h>
#include <iostream>
#include "passwordgen.h"
#include <dpp/fmt/format.h>
#include "Utils.h"
#include "EmbedBuilder.h"
#include "SQLrequests.h"
#include <cstdlib> 
#include <map>
#include "MyBot.h"
#include <thread>
#include "VoiceRecording.h"
#include "HTTPrequests.h"
#include "nlohmann/json.hpp"
#include <fstream>
using namespace dpp;
using json = nlohmann::json;
std::map<std::string, int> loggedUsers;
snowflake user_to_rec;
bool shouldRec;

int main()
{
    try {
        user_to_rec = 0;
        shouldRec = false;
        std::thread resettask (resetloggedusers);

        std::ifstream i("config.json");
        json j;
        i >> j;
        
        
        

        dpp:cluster bot(j["Token"]);

        
        

        bot.on_ready([&bot](const dpp::ready_t& event) {

            std::ifstream i("config.json");
            json j;
            i >> j;


            auto servid = j["Serv_id"];
            //bot.log(dpp::ll_info, "Connecte en tant que " + bot.me.username);
            Log(u8"Connecté en tant que " + bot.me.username);
            dpp::slashcommand passwcommand;
            passwcommand.set_name("password")
                .set_description(u8"Vous permet de générer un mot de passe aléatoire")
                .set_application_id(bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "char", "Le nombre de characters que vous souhaitez dans votre mdp", true)
                );
            dpp::slashcommand registercmd;
            registercmd.set_name("register")
                .set_description(u8"Vous permet de vous enregistrer en définissant un MasterPassword")
                .set_application_id(bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "password", u8"Un mot de passe que seul vous connaissez", true)
                );
            dpp::slashcommand logincmd;
            logincmd.set_name("login")
                .set_description(u8"Vous permet de vous identifier grâce à votre MasterPassword")
                .set_application_id(bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "password", u8"Votre MasterPassword", true)
                );
            dpp::slashcommand renamecmd;
            renamecmd.set_name("rename")
                .set_description(u8"Vous permet de renommer un mot de passe précédemment enregistré")
                .set_application_id(bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "id", u8"L'ID actuel de votre mot de passe", true)

                ).add_option(
                    dpp::command_option(dpp::co_string, "new_id", u8"Le nouveau nom souhaité pour ce mot de passe", true)
                );
            dpp::slashcommand sendcmd;
            sendcmd.set_name("sendpassword")
                .set_description(u8"Vous envoi en message privés la liste de vos mots de passes")
                .set_application_id(bot.me.id);
            dpp::slashcommand meteocmd;
            meteocmd.set_name("meteo")
                .set_description(u8"Vous permet de renvoyer la météo d'un endroit")
                .set_application_id(bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "location", u8"La ville pour laquelle vous souhaitez connaître la météo", true)
                );
            bot.guild_command_create(passwcommand, servid);
            bot.guild_command_create(registercmd, servid);
            bot.guild_command_create(logincmd, servid);
            bot.guild_command_create(renamecmd, servid);
            bot.guild_command_create(sendcmd, servid);
            bot.guild_command_create(meteocmd, servid);
                
        });

        bot.on_interaction_create([&bot](const dpp::interaction_create_t& event) {
            if (event.command.type == dpp::it_application_command) {
                dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);
                
                if (cmd_data.name == "password") {
                   

                    std::string chars = std::get<std::string>(event.get_parameter("char"));
                    int number_of_char = std::stoi(chars);
                    
                    Generate(number_of_char, event);
                }
                else if (cmd_data.name == "register")
                {
                    if (!CheckIfRegistered(to_string(event.command.usr.id)))
                    {
                        dpp::message msg = dpp::message().set_content(u8"*__:white_check_mark:Ton MasterPassword a bien été défini !__*").set_flags(m_ephemeral);
                        event.reply(dpp::ir_deferred_channel_message_with_source, msg);

                        std::string psw = std::get<std::string>(event.get_parameter("password"));
                        InsertMPWD(to_string(event.command.usr.id), psw);
                        event.edit_response(msg);
                    }
                    else
                    {
                        dpp::message msg = dpp::message().set_content(u8"*__:x:Ton MasterPassword a déjà été défini au préalable, utilise la commande `/reset` si tu souhaite le réinitialiser !__*").set_flags(m_ephemeral);
                        event.reply(dpp::ir_deferred_channel_message_with_source, msg);

                        event.edit_response(msg);
                    }
                }
                else if (cmd_data.name == "login")
                {
                    std::string psw = std::get<std::string>(event.get_parameter("password"));
                    std::string id = to_string(event.command.usr.id);
                    if (CheckLogin(psw, id))
                    {
                        dpp::message msg = dpp::message().set_content(u8"*__:white_check_mark: Tu es maintenant connecté pour une période de 15 minutes !__*").set_flags(m_ephemeral);
                        event.reply(dpp::ir_deferred_channel_message_with_source, msg);
                        loggedUsers[id] = 1;
                        event.edit_response(msg);
                    }
                    else
                    {
                        dpp::message msg = dpp::message().set_content(u8"*__:x:Ton MasterPassword est invalide ou tu ne t'es pas encore `/register` !__*").set_flags(m_ephemeral);
                        event.reply(dpp::ir_deferred_channel_message_with_source, msg);

                        event.edit_response(msg);
                    }
                }
                else if (cmd_data.name == "rename")
                {
                    std::string id = to_string(event.command.usr.id);
                    if (loggedUsers.count(id))
                    {
                        std::string pass_id = std::get<std::string>(event.get_parameter("id"));
                        std::string new_id = std::get<std::string>(event.get_parameter("new_id"));
                        if (CheckIfExit(id, pass_id))
                        {
                            PasswordRegistered(event, new_id);
                            RenamePWD(id, pass_id, new_id);
                        }
                        else
                        {
                            NoPassFound(event, pass_id);
                        }
                    }
                    else
                    {
                        AskMasterPassword(event);
                    }
                }
                else if (cmd_data.name == "sendpassword")
                {
                    std::string id = to_string(event.command.usr.id);
                    if (loggedUsers.count(id))
                    {
                        GetPasswords(id);
                        SendPassword(event.command.usr, bot);
                        Sleep(1000);
                        passwords.clear();
                    }
                    else
                    {
                        AskMasterPassword(event);
                    }
                }
                else if (cmd_data.name == "meteo")
                {
                    std::string location = std::get<std::string>(event.get_parameter("location"));
                    GetMeteo(location);
                    SendMeteo(event);
                }
            }
            });

        bot.on_button_click([&bot](const dpp::button_click_t& event) {
            if (event.custom_id.at(0) == 'W')
            {
                
                std::string id = split(event.custom_id, "_")[1];
                std::string passw = split(event.custom_id, "_")[2];
                if (id == to_string(event.command.usr.id))
                {
                    PasswordSentAlert(event);
                    Password(event.command.usr, bot, passw);
                    
                    
                }
                else
                {
                    NotTargetedAlert(event);
                }
                
            }
            else if (event.custom_id.at(0) == 'Q')
            {
              
                std::string id = split(event.custom_id, "_")[1];
                std::string passw = split(event.custom_id, "_")[2];
                
                
                if (CheckIfRegistered(id))
                {
                    if (loggedUsers.count(id))
                    {
                        srand(time(0));
                        std::string identifier = to_string(rand() % 500);
                        InsertPWD(id, passw, identifier);
                        PasswordRegistered(event, identifier);
                    }
                    else
                    {
                        AskMasterPassword(event);
                    }
                    
                }
                else
                {
                    AskCreatePassword(event);
                }   
            }
          
          
            });

        bot.on_message_create([&bot](const dpp::message_create_t& event) {
            
            if (event.msg.content == "!ping") {
                std::string time = to_string(bot.rest_ping);
                std::cout << time;
                std::string pong = time + " seconds";
                std::cout << pong;
                bot.message_create(dpp::message(event.msg.channel_id, pong));
            }
            else if (event.msg.content == "!record")
            {
                dpp::guild* g = dpp::find_guild(event.msg.guild_id);
                if (!g->connect_member_voice(event.msg.author.id)) {
                    bot.message_create(dpp::message(
                        event.msg.channel_id,
                        "Tu n'es dans aucun channel vocal:("
                    ));
                }
                else
                {
                    bot.message_create(dpp::message(
                        event.msg.channel_id,
                        "Je commence l'enregistrement"
                    ));
                    user_to_rec = event.msg.author.id;
                }

            }
            else if (event.msg.content == "!stop")
            {
                event.from->disconnect_voice(event.msg.guild_id);
                fclose(fd);
            }
        });

        bot.on_voice_receive([&bot](const dpp::voice_receive_t& event) {
                Record(bot, event);
            });

        bot.on_log([](const dpp::log_t& event) {
            if (event.severity > dpp::ll_error) {
                std::cout << dpp::utility::loglevel(event.severity) << ": " << event.message << "\n";
            }
        });

        bot.start(false);
        
          
        
        
        
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
