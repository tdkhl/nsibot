#include <cstdio>
extern FILE* fd;

#ifndef VOICERECORDING_H
#define VOICERECORDING_H
int Record(dpp::cluster& bot, const dpp::voice_receive_t& event);
#endif