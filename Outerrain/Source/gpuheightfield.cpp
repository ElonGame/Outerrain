#include "heightfield.h"
#include "mathUtils.h"
#include <chrono>

GPUHeightfield::GPUHeightfield() : Heightfield("Data/Heightmaps/dome-2048.png", 0, 500, 1024, 1024, Box2D(Vector2(-1024, -1024), Vector2(1024, 1024)))
{
	heightIntegerData.resize(values.size());
	computeShader.InitFromFile("Shaders/HeightfieldCompute.glsl");
	computeShader.PrintCompileErrors();

	// Compute thread count
	GLuint computeShaderProgram = computeShader.GetProgram();
	GLint threads[3] = { };
	glGetProgramiv(computeShaderProgram, GL_COMPUTE_WORK_GROUP_SIZE, threads);
	threadGroupCount = values.size() / threads[0];
	if (values.size() % threads[0] > 0)
		threadGroupCount++;
}

GPUHeightfield::~GPUHeightfield()
{
	glDeleteBuffers(1, &dataBuffer);
	computeShader.Release();
}

void GPUHeightfield::GenerateBuffers()
{
	for (int i = 0; i < heightIntegerData.size(); i++)
		heightIntegerData[i] = (int)values[i]; //Math::ChangeBase(minHeightValue, maxHeightValue, 0.0f, 1000000.0f, values[i]);
	computeShader.Attach();
	glGenBuffers(1, &dataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * heightIntegerData.size(), &heightIntegerData.front(), GL_STREAM_COPY);
}

void GPUHeightfield::ThermalWeathering(float amplitude)
{
	if (false)
	{
		auto cpu_start = std::chrono::high_resolution_clock::now();
		Heightfield::ThermalWeathering(2.0f);
		auto cpu_stop = std::chrono::high_resolution_clock::now();
		long long int cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start).count();
		//std::cout << "CPU : " << static_cast<int>((cpu_time / 1000000)) << "ms" << std::endl;
	}
	else
	{
		//auto cpu_start = std::chrono::high_resolution_clock::now();

		GenerateBuffers();
		// Dispatch
		computeShader.Attach();
		glDispatchCompute(threadGroupCount, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		// Update data
		glGetNamedBufferSubData(dataBuffer, 0, sizeof(int) * heightIntegerData.size(), heightIntegerData.data());
		for (int i = 0; i < values.size(); i++)
		{
			float newVal = (float)heightIntegerData[i];
			values[i] = newVal; //Math::ChangeBase<float>(0.0f, 1000000.0f, minHeightValue, maxHeightValue, newVal);
		}
		glDeleteBuffers(1, &dataBuffer);

		//auto cpu_stop = std::chrono::high_resolution_clock::now();
		//long long int cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start).count();
		//std::cout << "CPU : " << static_cast<int>((cpu_time / 1000000)) << "ms" << std::endl;
	}
}
