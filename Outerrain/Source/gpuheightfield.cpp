#include "heightfield.h"

GPUHeightfield::GPUHeightfield() : Heightfield("Data/Heightmaps/island.png", 0, 100, 256, 256, Box2D(Vector2(-256, -256), Vector2(256, 256)))
{
	outputHeights.resize(values.size());

	computeShader.InitFromFile("Shaders/heightfieldCompute.glsl");
	computeShader.PrintCompileErrors();

	// Compute thread count
	GLuint computeShaderProgram = computeShader.GetProgram();
	GLint threads[3] = {};
	glGetProgramiv(computeShaderProgram, GL_COMPUTE_WORK_GROUP_SIZE, threads);
	threadGroupCount = values.size() / threads[0];
	if (values.size() % threads[0] > 0)
		threadGroupCount++; // un groupe supplémentaire, si N n'est pas un multiple de threads[0]
}

GPUHeightfield::~GPUHeightfield()
{
	glDeleteBuffers(1, &inputData);
	glDeleteBuffers(1, &outputData);
	computeShader.Release();
}

void GPUHeightfield::GenerateBuffers()
{
	computeShader.Attach();

	glGenBuffers(1, &inputData);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputData);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * values.size(), &values.front(), GL_STREAM_COPY);

	glGenBuffers(1, &outputData);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputData);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * outputHeights.size(), &outputHeights.front(), GL_STREAM_COPY);
}

void GPUHeightfield::ThermalWeathering(float amplitude)
{
	GenerateBuffers();

	// Dispatch
	computeShader.Attach();
	glDispatchCompute(threadGroupCount, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Update data
	glGetNamedBufferSubData(outputData, 0, sizeof(float) * outputHeights.size(), outputHeights.data());
	for (int i = 0; i < values.size(); i++)
		values[i] = outputHeights[i];

	glDeleteBuffers(1, &inputData);
	glDeleteBuffers(1, &outputData);
}
