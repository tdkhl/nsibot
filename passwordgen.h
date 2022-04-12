#include <string>
#include <dpp/dpp.h>
using namespace dpp;
#ifndef PASSWORDGEN_H
#define PASSWORDGEN_H

char GenRandom();
int Generate(int n, const dpp::interaction_create_t& event);



#endif
