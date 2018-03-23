#include "apptime.h"
#include "GL\glew.h"

std::chrono::high_resolution_clock::time_point AppTime::cpu_start;
std::chrono::high_resolution_clock::time_point AppTime::cpu_stop;
unsigned int AppTime::gpuTimeQuery = 0;

static bool first = false;

void AppTime::StartClock()
{
	if (!first)
	{
		glGenQueries(1, &gpuTimeQuery);
		first = false;
	}

	glBeginQuery(GL_TIME_ELAPSED, gpuTimeQuery);
	cpu_start = std::chrono::high_resolution_clock::now();
}

void AppTime::StopClock(std::stringstream& cpuStream, std::stringstream& gpuStream)
{
	cpu_stop = std::chrono::high_resolution_clock::now();
	long long int cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start).count();

	glEndQuery(GL_TIME_ELAPSED);
	GLint64 gpu_time = 0;
	glGetQueryObjecti64v(gpuTimeQuery, GL_QUERY_RESULT, &gpu_time);

	cpuStream.str("");
	gpuStream.str("");
	cpuStream << "CPU " << static_cast<int>((cpu_time / 1000000)) << "ms" << static_cast<int>(((cpu_time / 1000) % 1000)) << "us";
	gpuStream << "GPU " << static_cast<int>((gpu_time / 1000000)) << "ms" << static_cast<int>(((gpu_time / 1000) % 1000)) << "us";
}

void AppTime::Release()
{
	glDeleteQueries(1, &gpuTimeQuery);
}
