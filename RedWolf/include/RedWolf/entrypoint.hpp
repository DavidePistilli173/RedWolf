#ifndef RW_ENTRYPOINT_HPP
#define RW_ENTRYPOINT_HPP

#include <SDL2.0.9/SDL.h>

#include <memory>

extern std::unique_ptr<rw::App> rw::createApp();

/** 
    \brief Application entry point.
    The entry point of an application that uses RedWolf is handled by the engine itself.
*/
int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO);

    auto app{ rw::createApp() };
    app->run();

    SDL_Quit();
    return 0;
}

#endif