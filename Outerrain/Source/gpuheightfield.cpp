#include "gpuHeightfield.h"
#include "mathUtils.h"

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
		heightIntegerData[i] = (int)values[i];
	computeShader.Attach();
	glGenBuffers(1, &dataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * heightIntegerData.size(), &heightIntegerData.front(), GL_STREAM_COPY);
}

void GPUHeightfield::ThermalWeathering(float amplitude)
{
	// Prepare buffers
	GenerateBuffers();

	// Dispatch
	computeShader.Attach();
	glDispatchCompute(threadGroupCount, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	// Update CPU data
	glGetNamedBufferSubData(dataBuffer, 0, sizeof(int) * heightIntegerData.size(), heightIntegerData.data());
	for (int i = 0; i < values.size(); i++)
	{
		float newVal = (float)heightIntegerData[i];
		values[i] = newVal;
	}
	glDeleteBuffers(1, &dataBuffer);
}
