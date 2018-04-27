#include "mytime.h"
#include <SDL2/SDL.h>

unsigned int Time::last_time;
unsigned int Time::last_delta;

float Time::GlobalTime()
{
    unsigned int now = SDL_GetTicks();
    if (now <= last_time)
        now = last_time + 1;
    last_delta = now - last_time;
    last_time = now;
    return float(last_time);
}

float Time::DeltaTime()
{
    return float(last_delta);
}
