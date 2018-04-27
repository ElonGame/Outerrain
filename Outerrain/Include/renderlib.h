#pragma once
#include <vector>
#include <GL\glew.h>

#include "mesh.h"
#include "material.h"
#include "vec.h"
#include "cameraOrbiter.h"

struct VertexInfo
{
public:
	Vector3 position;
	Vector2 texcoord;
	Vector3 normal;
};

template<typename T>
class GenericBuffer
{
private:
	std::vector<T> buffer;
	GLuint bufferIndex;

public:
	GenericBuffer() { bufferIndex = 0; }
	GenericBuffer(const std::vector<T>& b) : GenericBuffer() { buffer = b; Allocate(); }
	~GenericBuffer() { glDeleteBuffers(1, &bufferIndex); }

	void Allocate();
	void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferIndex); }
	void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	size_t GetBufferSize() const { return buffer.size(); }
	GLuint GetBufferIndex() const { return bufferIndex; }
	const std::vector<T>& GetBufferData() const { return buffer; }
};

class VertexBuffer
{
private:
	std::vector<VertexInfo> buffer;
	GLuint bufferIndex;

public:
	VertexBuffer() { bufferIndex = 0; }
	VertexBuffer(const std::vector<VertexInfo>& b) : VertexBuffer() { buffer = b; Allocate(); }
	~VertexBuffer() { glDeleteBuffers(1, &bufferIndex); }

	void Allocate();
	void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferIndex); }
	void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	GLuint GetBufferIndex() const { return bufferIndex; }
	const std::vector<VertexInfo>& GetBufferData() const { return buffer; }
};

class MeshRender
{
private:
	GLuint vao;
	GenericBuffer<unsigned int> indexBuffer;
	VertexBuffer vertexBuffer;

	Material mat;

public:
	MeshRender(const Mesh& m);
	~MeshRender();

	void SetMaterial(const Material& m) { mat = m; }
	void Draw(const CameraOrbiter& cam);
};
