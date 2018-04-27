#include "gpuHeightfield.h"
#include "mathUtils.h"

/*
\brief Constructor
*/
GPUHeightfield::GPUHeightfield() : HeightField()
{
	InitGPUPrograms();
}

/*
\brief Constructor
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
*/
GPUHeightfield::GPUHeightfield(int nx, int ny, const Box2D& bbox) : HeightField(nx, ny, bbox)
{
	InitGPUPrograms();
}

/*
\brief Constructor
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
\param value default value for the field
*/
GPUHeightfield::GPUHeightfield(int nx, int ny, const Box2D& bbox, float value) : HeightField(nx, ny, bbox, value)
{
	InitGPUPrograms();
}

/*
\brief Constructor from a file
\param file file path
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
*/
GPUHeightfield::GPUHeightfield(const std::string& file, float minAlt, float maxAlt, int nx, int ny, const Box2D& bbox) : HeightField(file, minAlt, maxAlt, nx, ny, bbox)
{
	InitGPUPrograms();
}

/*
\brief Constructor from a noise
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
\param amplitude noise amplitude
\param freq noise frequency
\param oct noise octave count
\param type fractal type. See enum.
*/
GPUHeightfield::GPUHeightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, FractalType type) : HeightField(nx, ny, bbox, n, amplitude, freq, oct, type)
{
	InitGPUPrograms();
}

/*
\brief Constructor from a noise
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
\param amplitude noise amplitude
\param freq noise frequency
\param oct noise octave count
\param offset noise offset vector
\param type fractal type. See enum.
*/
GPUHeightfield::GPUHeightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type) : HeightField(nx, ny, bbox, n, amplitude, freq, oct, offset, type)
{
	InitGPUPrograms();
}

/*
\brief Constructor from TerrainSettings class for convenience and consistancy.
\param settings TerrainSettings for the heightfield.
*/
GPUHeightfield::GPUHeightfield(const TerrainSettings& settings) : HeightField(settings)
{
	InitGPUPrograms();
}

/*
\brief Destructor. Delete buffers and release compute shader program.
*/
GPUHeightfield::~GPUHeightfield()
{
	glDeleteBuffers(1, &floatingDataBuffer);
	computeShader.Release();
}

/*
\brief Load the compute shaders and compute thread group count. This function prints errors if needed.
*/
void GPUHeightfield::InitGPUPrograms()
{
	computeShader = Shader("Shaders/HeightfieldCompute.glsl");
	computeShader.PrintCompileErrors();

	// Compute thread count
	GLuint computeShaderProgram = computeShader.GetProgram();
	GLint threads[3] = {};
	glGetProgramiv(computeShaderProgram, GL_COMPUTE_WORK_GROUP_SIZE, threads);
	threadGroupCount = int(values.size()) / threads[0];
	if (values.size() % threads[0] > 0)
		threadGroupCount++;
}

/*
\brief Allocate floating point data buffer on the gpu for the heightfield.
*/
void GPUHeightfield::AllocateBuffers()
{
	computeShader.Attach();
	glGenBuffers(1, &floatingDataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, floatingDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * values.size(), &values.front(), GL_STREAM_COPY);
}

/*
\brief Performs a thermal weathering erosion step. This function allocates and deletes buffers in the same frame.
\param amplitude max erosion amplitude
\param tanThresholdAngle tangent of the repose angle of the material.
*/
void GPUHeightfield::ThermalWeathering(float amplitude, float tanThresholdAngle)
{
	// Allocate buffers
	AllocateBuffers();

	// Uniforms
	computeShader.Attach();
	computeShader.UniformInt("gridSize", nx);
	computeShader.UniformFloat("amplitude", amplitude);
	computeShader.UniformFloat("tanThresholdAngle", tanThresholdAngle);
	computeShader.UniformFloat("cellSize", CellSize().x);

	// Dispatch
	glDispatchCompute(threadGroupCount, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	// Update CPU data
	glGetNamedBufferSubData(floatingDataBuffer, 0, sizeof(float) * values.size(), values.data());

	// Delete buffers
	glDeleteBuffers(1, &floatingDataBuffer);
}
