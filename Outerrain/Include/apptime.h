#pragma once
#include <chrono>
#include <sstream>
#include "GL\glew.h"

class AppTime
{
private:
	static std::chrono::high_resolution_clock::time_point cpu_start;
	static std::chrono::high_resolution_clock::time_point cpu_stop;
	static GLuint gpuTimeQuery;

public:
	static void StartClock();
	static void StopClock(std::stringstream& cpuStream, std::stringstream& gpuStream);
	static void Release();
};
