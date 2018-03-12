#include "heightfield.h"
#include "mathUtils.h"

GPUHeightfield::GPUHeightfield() : Heightfield("Data/Heightmaps/island.png", 0, 100, 256, 256, Box2D(Vector2(-256, -256), Vector2(256, 256)))
{
	inputHeights.resize(values.size());
	outputHeights.resize(values.size());

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
	glDeleteBuffers(1, &inputDataBuffer);
	glDeleteBuffers(1, &outputDataBuffer);
	computeShader.Release();
}

void GPUHeightfield::GenerateBuffers()
{
	minHeightValue = Min();
	maxHeightValue = Max();
	for (int i = 0; i < inputHeights.size(); i++)
	{
		inputHeights[i] = Math::ChangeBase(minHeightValue, maxHeightValue, 0.0f, 100000.0f, values[i]);
		outputHeights[i] = inputHeights[i];
	}

	computeShader.Attach();

	glGenBuffers(1, &inputDataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * inputHeights.size(), &inputHeights.front(), GL_STREAM_COPY);

	glGenBuffers(1, &outputDataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * outputHeights.size(), &outputHeights.front(), GL_STREAM_COPY);
}

void GPUHeightfield::ThermalWeathering(float amplitude)
{
	//Heightfield::ThermalWeathering(amplitude);
	
	GenerateBuffers();

	// Dispatch
	computeShader.Attach();
	glDispatchCompute(threadGroupCount, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Update data
	glGetNamedBufferSubData(outputDataBuffer, 0, sizeof(int) * outputHeights.size(), outputHeights.data());
	for (int i = 0; i < values.size(); i++)
	{
		float newVal = (float)outputHeights[i];
		values[i] = Math::ChangeBase<float>(0.0f, 100000.0f, minHeightValue, maxHeightValue, newVal);
	}

	glDeleteBuffers(1, &inputDataBuffer);
	glDeleteBuffers(1, &outputDataBuffer);
}
