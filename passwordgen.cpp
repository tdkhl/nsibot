#include <iostream>
#include <cstdlib>
#include <ctime>
#include "passwordgen.h"
#include <dpp/dpp.h>
#include <iostream>

using namespace dpp;
using namespace std;

static const char alphabet_de_nombre[] = "0123456789" "!%/@-#{}$*?"
"AZERTYUIOPQSDFGHLMWXCVBN" "azertyuiopqsdfghjklmwxcvbn";
int strLen = sizeof(alphabet_de_nombre) - 1;
char GenRandom()
{

	int random = rand() % strLen;
	char result = alphabet_de_nombre[random];
	return result;

}
int Generate(int n, const dpp::interaction_create_t& event)
{
	int c = 0, s = 0;
	srand(time(0));
N:
	char C;
	string D;

	for (int z = 0; z < n; z++)
	{
		C = GenRandom();
		D += C;
		if (isdigit(C))
		{
			c++;
		}
		if (C == '!' || C == '£' || C == 'ù' || C == '%' || C == '/' || C == '@' || C == '-' || C == '#'
			|| C == '{' || C == '}' || C == '$' || C == '*' || C == '?')
		{
			s++;
		}
	}
	if (n > 2 && (s == 0) || c == 0)
	{
		goto N;
	}



	string id_string = to_string(event.command.usr.id);
	dpp::embed embed = dpp::embed().
		set_color(0x33B4D4).
		set_title(u8"Nouveau mot de passe généré!").
		set_description("***__Cliquez sur le cadenas ci-dessous afin de recevoir votre mot de passe en DM !__***");
	dpp::message msg = dpp::message().add_embed(embed).add_component(
		dpp::component().add_component(
			dpp::component().set_type(dpp::cot_button).
			set_emoji(u8"🔐").
			set_style(dpp::cos_secondary).
			set_id("Wsendpassword_" + id_string + "_" + D)
		)
	);
	event.reply(dpp::ir_channel_message_with_source, msg);
	return 0;


}



