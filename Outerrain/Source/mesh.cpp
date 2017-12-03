#include "mesh.h"

#include <cassert>


Mesh::Mesh() : 
	vertices(), texcoords(), normals(), colors(), indices(), 
	primitiveDrawn(GL_POINTS), VAO(0), fullBuffer(0), indexBuffer(0), 
	shaderProgram(0) { }

void Mesh::AddVertex(const Vector3& v)
{
	vertices.push_back(v);
	updateBuffersNextDraw = true;
}

void Mesh::AddVertex(const int& i, const Vector3& v)
{
	assert(i < vertices.size());
	vertices[i] = v;
	updateBuffersNextDraw = true;
}

void Mesh::AddNormal(const Vector3& n)
{
	normals.push_back(n);
	updateBuffersNextDraw = true;
}

void Mesh::AddNormal(const int& i, const Vector3& v)
{
	assert(i < normals.size());
	normals[i] = v;
	updateBuffersNextDraw = true;
}

void Mesh::AddColor(const Vector4& v)
{
	colors.push_back(v);
	updateBuffersNextDraw = true;
}

void Mesh::AddColor(const int& i, const Vector4& v)
{
	assert(i < colors.size());
	updateBuffersNextDraw = true;
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
	updateBuffersNextDraw = true;
}

void Mesh::SetShader(GLuint shader)
{
	shaderProgram = shader;
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &fullBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Mesh::CreateBuffers(const bool use_texcoord, const bool use_normal, const bool use_color)
{
	if (vertices.size() == 0)
		return;

	if (use_texcoord && texcoords.size() > 0 && texcoords.size() < vertices.size())
		printf("[error] invalid texcoords array...\n");
	if (use_normal && normals.size() > 0 && normals.size() < vertices.size())
		printf("[error] invalid normals array...\n");
	if (use_color && colors.size() > 0 && colors.size() < vertices.size())
		printf("[error] invalid colors array...\n");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	size_t size = VertexBufferSize() + TexcoordBufferSize() + NormalBufferSize() + ColorBufferSize();
	glGenBuffers(1, &fullBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, fullBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	size_t offset = 0;
	size = VertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, VertexBufferPtr());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
	glEnableVertexAttribArray(0);

	if (texcoords.size() == vertices.size() && use_texcoord)
	{
		offset = offset + size;
		size = TexcoordBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, TexcoordBufferPtr());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
		glEnableVertexAttribArray(1);
	}

	if (normals.size() == vertices.size() && use_normal)
	{
		offset = offset + size;
		size = NormalBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, NormalBufferPtr());
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
		glEnableVertexAttribArray(2);
	}

	if (colors.size() == vertices.size() && use_color)
	{
		offset = offset + size;
		size = ColorBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, ColorBufferPtr());
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
		glEnableVertexAttribArray(3);
	}

	if (IndexBufferSize())
	{
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize(), IndexBufferPtr(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::UpdateBuffers(const bool use_texcoord, const bool use_normal, const bool use_color)
{
	assert(VAO > 0);
	assert(fullBuffer > 0);
	if (updateBuffersNextDraw == false)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, fullBuffer);
	size_t offset = 0;
	size_t size = VertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, VertexBufferPtr());

	if (texcoords.size() == vertices.size() && use_texcoord)
	{
		offset = offset + size;
		size = TexcoordBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, TexcoordBufferPtr());
	}

	if (normals.size() == vertices.size() && use_normal)
	{
		offset = offset + size;
		size = NormalBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, NormalBufferPtr());
	}

	if (colors.size() == vertices.size() && use_color)
	{
		offset = offset + size;
		size = ColorBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, ColorBufferPtr());
	}

	updateBuffersNextDraw = false;
}

void Mesh::Draw()
{
	if (VAO == 0)
		CreateBuffers(true, true, true);
	assert(VAO != 0);

	if (updateBuffersNextDraw == true)
		UpdateBuffers(true, true, true);

	glBindVertexArray(VAO);
	glUseProgram(shaderProgram);

	if (indices.size() > 0)
		glDrawElements(primitiveDrawn, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(primitiveDrawn, 0, (GLsizei)vertices.size());
}