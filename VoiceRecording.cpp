#include <dpp/dpp.h>
#include <iomanip>
#include <sstream>
#include "MyBot.h";
#include "VoiceRecording.h";
#include "Utils.h";
FILE* fd;
int Record(dpp::cluster& bot, const dpp::voice_receive_t& event)
{
    std::cout << user_to_rec  << "1" << std::endl;
    std::cout << event.user_id << "2";
    if (event.user_id == user_to_rec)
    {

        errno_t err;
        std::ostringstream oss;
        srand(time(0));

        oss << "./" << std::to_string(user_to_rec) << "_" << std::to_string(rand() % 500) << ".pcm";
        const char* path = oss.str().c_str();
        err = fopen_s(&fd, path, "w+");
        if (err = 0)
        {
            std::cout << "worked";
            fwrite((char*)event.audio, 1, event.audio_size, fd);
        }



        
    }
    

    

    return 0;
}