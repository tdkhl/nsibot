#include <dpp/dpp.h>
#include <iostream>
#ifndef EMBEDBUILDER_H
#define EMBEDBUILDER_H

int PasswordSentAlert(const dpp::interaction_create_t& event);
int NotTargetedAlert(const dpp::interaction_create_t& event);
int Password(dpp::user user, dpp::cluster &bot, std::string passw);
int AskMasterPassword(const dpp::interaction_create_t& event);
int AskCreatePassword(const dpp::interaction_create_t& event);
int PasswordRegistered(const dpp::interaction_create_t& event, std::string id);
int NoPassFound(const dpp::interaction_create_t& event, std::string id);
int SendPassword(dpp::user user, dpp::cluster& bot);
int SendMeteo(const dpp::interaction_create_t& event);
#endif