#include "mytime.h"

unsigned int Time::last_time;
unsigned int Time::last_delta;

float Time::GlobalTime()
{
    unsigned int now = SDL_GetTicks();
    if (now <= last_time)
        now = last_time + 1;
    last_delta = now - last_time;
    last_time = now;
    return static_cast<float>(last_time);
    return 0.0f;
}

float Time::DeltaTime()
{
    return static_cast<float>(last_delta);
}
