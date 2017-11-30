#include "mesh.h"
#include <cassert>


void Mesh::AddVertex(const Vector3& v)
{
	vertices.push_back(v);
}

void Mesh::AddVertex(const int& i, const Vector3& v)
{
	assert(i < vertices.size());
	vertices[i] = v;
}

void Mesh::AddNormal(const Vector3& n)
{
	normals.push_back(n);
}

void Mesh::AddNormal(const int& i, const Vector3& v)
{
	assert(i < normals.size());
	normals[i] = v;
}

void Mesh::AddColor(const Vector4& v)
{
	colors.push_back(v);
}

void Mesh::AddColor(const int& i, const Vector4& v)
{
	assert(i < colors.size());
	colors[i] = v;
}

void Mesh::AddTriangle(const unsigned int a, const unsigned int b, const unsigned int c)
{
	assert(a < vertices.size());
	assert(b < vertices.size());
	assert(c < vertices.size());
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
}

Vector3 Mesh::Vertex(int i) const
{
	return vertices[i];
}

Vector3 Mesh::Normal(int i) const
{
	return normals[i];
}

Vector2 Mesh::Texcoord(int i) const
{
	return texcoords[i];
}

Vector4 Mesh::Color(int i) const
{
	return colors[i];
}

void Mesh::Destroy()
{

}