#include <iostream>
#include <cstdlib>
#include <ctime>
#include "EmbedBuilder.h"
#include <dpp/dpp.h>
#include <iostream>
#include <dpp/nlohmann/json.hpp>
#include <format>
#include "SQLrequests.h"
#include "HTTPrequests.h"
#include "Utils.h"
#include <iomanip>

using namespace dpp;
using namespace std;

int PasswordSentAlert(const dpp::interaction_create_t& event)
{

	dpp::message msg = dpp::message().set_content(u8"*__:white_check_mark: Votre mot de passe vous as été envoyé en message privé !__*").set_flags(m_ephemeral);
	
	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;
}

int NotTargetedAlert(const dpp::interaction_create_t& event)
{
	dpp::message msg = dpp::message().set_content(u8"*__:x: Oops, on dirait que tu n'a pas accès à ceci !__*").set_flags(m_ephemeral);
	

	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;
}

int Password(dpp::user user, dpp::cluster& bot, string passw)
{
	string id_string = to_string(user.id);
	dpp::embed embed = dpp::embed().
		set_color(0x33B4D4).
		set_title(u8"Voici ton mot de passe, assure-toi de ne pas être en partage d'écran !").
		set_description(u8"*__Cliquez afin de découvrir votre mot de passe__* ||" + passw + "||" + "\n :warning: *Si vous souhaitez enregistrer ce mot de passe, cliquez sur :inbox_tray: ci-dessous !*");
	const dpp::message msg = dpp::message().add_embed(embed).add_component(
		dpp::component().add_component(
			dpp::component().set_type(dpp::cot_button).
			set_emoji(u8"📥").
			set_style(dpp::cos_secondary).
			set_id("Qsavepwd_" + id_string + "_" + passw)
		)
	);

	bot.direct_message_create(user.id, msg);
	return 0;
}

int AskMasterPassword(const dpp::interaction_create_t& event)
{
	
	dpp::message msg = dpp::message().set_content(u8"*__:x:Tu dois exécuter la commande `/login [ton pass]` sur le serveur afin de pouvoir accéder à cette fonctionnalité, tu n'oubliera pas de remplacer ce qui est entre crochets par ton__* ***__MasterPassword__***").set_flags(m_ephemeral);
	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;
}

int AskCreatePassword(const dpp::interaction_create_t& event)
{
	
	dpp::message msg = dpp::message().set_content(u8"*__Afin d'enregistrer tes mots de passes, tu dois définir un__* ***__MasterPassword__*** *__grâce à la commande `/register [ton pass]` sur le serveur, une fois cela fais, ton__* ***__MasterPassword__*** *__sera créer !__*").set_flags(m_ephemeral);
	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;
}

int PasswordRegistered(const dpp::interaction_create_t& event, string id)
{
	std::string content = u8":white_check_mark: *__Ton mot de passe a bien été enregistré sous l'id `" + id + "` , utilise la commande `/rename[id][nouveau nom]` sur le serveur si tu souhaite changer son id !__* ";
	dpp::message msg = dpp::message().set_content(content).set_flags(m_ephemeral);
	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;
}

int NoPassFound(const dpp::interaction_create_t& event, string id)
{
	std::string content = u8":x: *__Aucun de vos mots de passe ne correspond à l'ID " + id + "__* ";
	dpp::message msg = dpp::message().set_content(content).set_flags(m_ephemeral);
	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;
}

int SendPassword(dpp::user user, dpp::cluster& bot)
{
	string id_string = to_string(user.id);
	dpp::embed embed = dpp::embed().
		set_color(0x9072F4).
		set_title(u8"Voici la liste de vos mots de passes enregistrés !").
		set_description(u8"*__Assurez vous de n'avoir personne autour de vous, de ne pas être en partage d'écran__*");
	for (int i = 0; i < passwords.size(); i++)
	{
		if (i > 15)
		{
			break;
		}
		string identifier = split(passwords[i], "_")[0];
		string pw = split(passwords[i], "_")[1];
		embed.add_field("*__" + identifier + "__*", "||*" + pw + "*||", false);
	};
		
	const dpp::message msg = dpp::message().add_embed(embed).set_flags(m_ephemeral);

	bot.direct_message_create(user.id, msg);
	return 0;
}

int SendMeteo(const dpp::interaction_create_t& event)
{

	try {
		string code = Meteo["code"]; // bon
		if (code == "success")
		{
			string name = Meteo["name"];
			string region = Meteo["region"];
			string Ensoleillement = Meteo["Ensoleillement"];
			string pays = Meteo["pays"];
			string icon = Meteo["icon"];
			string heure = Meteo["heure"];
			string update = Meteo["update"];
			string temp = Meteo["temp"] + u8"°";
			string ressenti = Meteo["ressenti"] + u8"°";
			string vent = Meteo["vent"];
			string precip = Meteo["precip"];
			string humid = Meteo["humid"];
			string cloud = Meteo["cloud"];
			string uv = Meteo["uv"];

			
			
			

			dpp::embed embed = dpp::embed().
				set_color(0xECB431).
				set_title(u8"Voici la météo pour " + name).
				set_description("***__" + Ensoleillement + ".__*** ");


			dpp::embed_footer footer = dpp::embed_footer().set_text("Derniere update :arrow_up: ");
			embed.set_footer(footer);


			embed.add_field("***__:tokyo_tower: Ville :__*** ", name, true);
			embed.add_field("***__:earth_africa: Region :__*** ", region, true);
			embed.add_field("***__:triangular_flag_on_post: Pays :__*** ", pays, true);
			embed.add_field(u8"***__:sunny: Température :__*** ", temp, true);
			embed.add_field(u8"***__:partly_sunny: Ressenti :__*** ", ressenti, true);
			embed.add_field(u8"***__:cloud_tornado: Vent :__*** ", vent + "KM/H", true);
			embed.add_field(u8"***__:cloud_rain: Précipitations :__*** ", precip + "mm", true);
			embed.add_field(u8"***__:droplet: Humidité :__*** ", humid + "%", true);
			embed.add_field(u8"***__:cloud: Ciel couvert :__*** ", cloud + "%", true);
			embed.add_field(u8"***__:sunglasses: Index UV :__*** ", uv, true);
			embed.add_field(u8"***__:clock11: Heure locale :__*** ", heure, true);
			embed.add_field(u8"***__Derniere update :__*** ", update, true);


			embed.set_thumbnail("https:" + icon);
			
			


			dpp::message msg = dpp::message().add_embed(embed);
			event.reply(dpp::ir_channel_message_with_source, msg);
			Meteo.clear();

		}
		else
		{
			dpp::message msg = dpp::message().set_content(u8":x: *__Aucune ville trouvée à ce nom, essayez de remplacer les espaces par un `-`, si le nom possède un accent, tentez de l'enlever__*");
			event.reply(dpp::ir_channel_message_with_source, msg);
			Meteo.clear();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}



