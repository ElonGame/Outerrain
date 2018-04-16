#pragma once
#include "heightfield.h"

class GPUHeightfield : public Heightfield
{
protected:
	Shader computeShader;
	GLuint integerDataBuffer;
	GLuint floatingDataBuffer;
	int threadGroupCount;

	void InitGPUPrograms();

public:
	GPUHeightfield();
	GPUHeightfield(const TerrainSettings& settings);
	GPUHeightfield(int nx, int ny, const Box2D& bbox);
	GPUHeightfield(int nx, int ny, const Box2D& bbox, float value);
	GPUHeightfield(const std::string& filePath, float minAltitude, float maxAltitude, int nx, int ny, const Box2D& bbox);
	GPUHeightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, FractalType type);
	GPUHeightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type);
	~GPUHeightfield();

	void GenerateBuffers();
	virtual void ThermalWeathering(float amplitude, float tanThresholdAngle = 0.6f);
};
