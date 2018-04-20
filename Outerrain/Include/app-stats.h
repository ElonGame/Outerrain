#pragma once
#include <chrono>
#include <sstream>

class AppStatistics
{
private:
	static std::chrono::high_resolution_clock::time_point cpu_start;
	static std::chrono::high_resolution_clock::time_point cpu_stop;
	static unsigned int gpuTimeQuery;

public:
	static void StartClock();
	static void StopClock(std::stringstream& cpuStream, std::stringstream& gpuStream);
	static void Release();

	static unsigned int vertexCount;
	static unsigned int triangleCount;
};
