#pragma once
#include <vector>
#include <GL\glew.h>
#include "vec.h"
#include "shader.h"
#include "material.h"
#include "component.h"

class Terrain2D;
class CameraOrbiter;
struct ImageData;

enum RenderMode
{
	DiffuseMode = 0,
	NormalMode = 1,
	WireframeMode = 2,
	Texture = 3,
};

class Mesh : public Component
{
protected:
	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	std::vector<Vector4> colors;
	std::vector<unsigned int> indices;

	Material material;
	RenderMode renderMode;

	GLenum primitiveDrawn;
	GLuint VAO;
	GLuint fullBuffer;
	GLuint indexBuffer;
	GLuint texture0;
	
	Shader shader;
	bool updateBuffersNextDraw;

	void CreateBuffers(const bool use_texcoord = true, const bool use_normal = true, const bool use_color = true);
	void UpdateBuffers(const bool use_texcoord = true, const bool use_normal = true, const bool use_color = true);
	
	const float* VertexBufferPtr() const { return &vertices.front().x; }
	std::size_t VertexBufferSize() const { return vertices.size() * sizeof(Vector3); }

	const float *NormalBufferPtr() const { return &normals.front().x; }
	size_t NormalBufferSize() const { return normals.size() * sizeof(Vector3); }

	const float *TexcoordBufferPtr() const { return &texcoords.front().x; }
	size_t TexcoordBufferSize() const { return texcoords.size() * sizeof(Vector2); }

	const float *ColorBufferPtr() const { return &colors.front().x; }
	size_t ColorBufferSize() const { return colors.size() * sizeof(Vector4); }

	const void *IndexBufferPtr() const { return &indices.front(); }
	size_t IndexBufferSize() const { return indices.size() * sizeof(unsigned int); }

	GLuint MakeTexture(const int unit, const ImageData& im, const GLenum& texel_type);

public:
	Mesh();
	Mesh(const GLenum& primitives);
	~Mesh() { Destroy(); }

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
	void SetMaterial(const Material&);
	void SetRenderMode(const RenderMode&);
	void SetVertices(const std::vector<Vector3>&);

	void WriteMesh(const char*);
	void ReadMesh(const char*);

	Shader& GetShader() { return shader; }
	Bounds GetBounds() const;
	Vector3 Vertex(int) const;
	Vector3 Normal(int) const;
	Vector2 Texcoord(int) const;
	Vector4 Color(int) const;

	void CalculateNormals();
	void CalculateFromTerrain2D(const Terrain2D&);

	void Draw();
	void Draw(const CameraOrbiter&);

	void SetTexture(const char* filename, const GLenum& texel_type);
	void SetTexture(const GLuint& t) { texture0 = t; }
};
