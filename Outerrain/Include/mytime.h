#pragma once
#include <SDL2/SDL.h>

class Time
{
private:
	static unsigned int last_time;
	static unsigned int last_delta;
	Time();

public:
	static float GlobalTime();
	static float DeltaTime();
};