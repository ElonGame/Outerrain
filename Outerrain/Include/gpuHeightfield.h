#pragma once
#include "heightfield.h"

class GPUHeightfield : public Heightfield
{
protected:
	std::vector<int> heightIntegerData;

	Shader computeShader;
	GLuint dataBuffer;
	int threadGroupCount;

public:
	GPUHeightfield();
	~GPUHeightfield();

	void GenerateBuffers();
	virtual void ThermalWeathering(float amplitude);
};
