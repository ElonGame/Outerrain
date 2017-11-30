#pragma once
#include <vector>
#include "vec.h"

class Mesh
{
protected:
	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	std::vector<Vector4> colors;

	std::vector<unsigned int> indices;

public:
	Mesh() { }

	void Destroy();

	void AddVertex(const Vector3&);
	void AddVertex(const int&, const Vector3&);
	void AddNormal(const Vector3&);
	void AddNormal(const int&, const Vector3&);
	void AddColor(const Vector4&);
	void AddColor(const int&, const Vector4&);
	void AddTriangle(const unsigned int, const unsigned int, const unsigned int);

	Vector3 Vertex(int) const;
	Vector3 Normal(int) const;
	Vector2 Texcoord(int) const;
	Vector4 Color(int) const;
};
