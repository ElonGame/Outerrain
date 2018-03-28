#pragma once

#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>

#include "component.h"
#include "vec.h"
#include "box.h"

class Mesh : public Component
{
	friend class MeshRenderer;
	friend class MeshSetRenderer;

protected:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::vector<unsigned int> indices;

	bool isDirty;

public:
	Mesh();
	Mesh(const std::string& path);

	void AddVertex(const Vector3&);
	void AddNormal(const Vector3&);
	void AddTriangle(unsigned int, unsigned int, unsigned int);
	void AddTexcoord(const Vector2&);

	Box GetBounds() const;
	bool LoadObj(const std::string& path);
	void ClearBuffers();
	void PrintInfos();

	std::vector<Vector3> Normals() const { return normals; }
	std::vector<Vector3> Vertices() const { return vertices; }

	size_t VertexCount() const { return vertices.size(); }
	size_t NormalCount() const { return normals.size(); }
	size_t TexcoordCount() const { return texcoords.size(); }
	size_t TriangleCount() const { return indices.size(); }

	const float* VertexBufferPtr() const { return &vertices.front().x; }
	size_t VertexBufferSize() const { return vertices.size() * sizeof(Vector3); }

	const float* NormalBufferPtr() const { return &normals.front().x; }
	size_t NormalBufferSize() const { return normals.size() * sizeof(Vector3); }

	const float* TexcoordBufferPtr() const { return &texcoords.front().x; }
	size_t TexcoordBufferSize() const { return texcoords.size() * sizeof(Vector2); }

	const void* IndexBufferPtr() const { return &indices.front(); }
	size_t IndexBufferSize() const { return indices.size() * sizeof(unsigned int); }
};
