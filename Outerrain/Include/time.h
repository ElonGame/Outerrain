#pragma once
#include <SDL2\SDL.h>

class Time
{
private:
	static unsigned int last_time;
	static unsigned int last_delta;
	Time();

public:
	static float GlobalTime()
	{
		unsigned int now = SDL_GetTicks();
		if (now <= last_time)
			now = last_time + 1;
		last_delta = now - last_time;
		last_time = now;
		return static_cast<float>(last_time);
	}
	
	static float DeltaTime()
	{
		return static_cast<float>(last_delta);
	}
};