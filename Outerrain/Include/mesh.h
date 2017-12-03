#pragma once
#include <vector>
#include <GL\glew.h>
#include "vec.h"
#include "shader.h"

class Mesh
{
protected:
	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	std::vector<Vector4> colors;
	std::vector<unsigned int> indices;

	GLenum primitiveDrawn;
	GLuint VAO;
	GLuint fullBuffer;
	GLuint indexBuffer;
	
	Shader shader;
	bool updateBuffersNextDraw;


	void CreateBuffers(const bool use_texcoord = true, const bool use_normal = true, const bool use_color = true);
	void UpdateBuffers(const bool use_texcoord = true, const bool use_normal = true, const bool use_color = true);

	const double* VertexBufferPtr() const { return &vertices.front().x; }
	std::size_t VertexBufferSize() const { return vertices.size() * sizeof(Vector3); }

	const double *NormalBufferPtr() const { return &normals.front().x; }
	size_t NormalBufferSize() const { return normals.size() * sizeof(Vector3); }

	const double *TexcoordBufferPtr() const { return &texcoords.front().x; }
	size_t TexcoordBufferSize() const { return texcoords.size() * sizeof(Vector2); }

	const double *ColorBufferPtr() const { return &colors.front().x; }
	size_t ColorBufferSize() const { return colors.size() * sizeof(Vector4); }

	const void *IndexBufferPtr() const { return &indices.front(); }
	size_t IndexBufferSize() const { return indices.size() * sizeof(unsigned int); }

public:
	Mesh();

	void Destroy();

	void AddVertex(const Vector3&);
	void AddVertex(const int&, const Vector3&);
	void AddNormal(const Vector3&);
	void AddNormal(const int&, const Vector3&);
	void AddColor(const Vector4&);
	void AddColor(const int&, const Vector4&);
	void AddTriangle(const unsigned int, const unsigned int, const unsigned int);
	void AddTexcoord(const Vector2&);
	void AddTexcoord(const int&, const Vector2&);
	void SetShader(const Shader&);

	Vector3 Vertex(int) const;
	Vector3 Normal(int) const;
	Vector2 Texcoord(int) const;
	Vector4 Color(int) const;

	void Draw();
};
