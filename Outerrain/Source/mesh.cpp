#include "mesh.h"
#include "cameraOrbiter.h"
#include "shader.h"
#include "terrain.h"
#include "gameobject.h"
#include "image.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
using namespace std;


Mesh::Mesh() :
	vertices(), texcoords(), normals(), colors(), indices(),
	primitiveDrawn(GL_TRIANGLES), VAO(0), fullBuffer(0), indexBuffer(0),
	shader(), material(Color::Green(), 32), renderMode(DiffuseMode) { }

Mesh::Mesh(const GLenum& primitives) :
	vertices(), texcoords(), normals(), colors(), indices(),
	primitiveDrawn(primitives), VAO(0), fullBuffer(0), indexBuffer(0),
	shader(), material(Color::Green(), 32), renderMode(DiffuseMode) { }

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

void Mesh::AddTexcoord(const Vector2& t)
{
	texcoords.push_back(t);
	updateBuffersNextDraw = true;
}

void Mesh::AddTexcoord(const int& i, const Vector2& t)
{
	assert(i < texcoords.size());
	updateBuffersNextDraw = true;
	texcoords[i] = t;
}

void Mesh::SetShader(const Shader& s)
{
	shader = s;
}

void Mesh::SetMaterial(const Material& m)
{
	material = m;
}

void Mesh::SetRenderMode(const RenderMode& r)
{
	renderMode = r;
}

void Mesh::SetVertices(const std::vector<Vector3>& verts)
{
	vertices.clear();
	normals.clear();
	for (int i = 0; i < verts.size(); i++)
		AddVertex(verts[i]);
	CalculateNormals();
	updateBuffersNextDraw = true;
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

Bounds Mesh::GetBounds() const
{
	Bounds ret;
	if (vertices.size() < 1)
		return ret;
	ret.min = Vector3(vertices[0]);
	ret.max = ret.min;
	for (unsigned int i = 1; i < (unsigned int)vertices.size(); i++)
	{
		Vector3 p = vertices[i];
		ret.min = Vector3(std::min(ret.min.x, p.x), std::min(ret.min.y, p.y), std::min(ret.min.z, p.z));
		ret.max = Vector3(std::max(ret.max.x, p.x), std::max(ret.max.y, p.y), std::max(ret.max.z, p.z));
	}
	return ret;
}

void Mesh::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &fullBuffer);
	glDeleteBuffers(1, &indexBuffer);
	shader.Release();
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

void Mesh::CalculateNormals()
{
	normals.resize(vertices.size());

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		// Sommets a, b, c de la face
		int a = indices[i];
		int b = indices[i + 1];
		int c = indices[i + 2];

		Vector3 AB = (vertices[b] - vertices[a]);
		Vector3 AC = (vertices[c] - vertices[a]);
		Vector3 normal = Cross(AB, AC);

		normals[a] = normals[a] + normal;
		normals[b] = normals[b] + normal;
		normals[c] = normals[c] + normal;
	}
	for (unsigned int i = 0; i < normals.size(); i++)
		normals[i] = Normalize(normals[i]);
}

void Mesh::CalculateFromTerrain2D(const Terrain2D& terrain)
{
	int nx = terrain.SizeX();
	int ny = terrain.SizeY();

	// Vertices & Texcoords & Normals
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / ((float)nx - 1);
			float v = i / ((float)ny - 1);
			AddVertex(terrain.Vertex(i, j));
			AddTexcoord(Vector2(u, v));
			AddNormal(terrain.Normal(i, j));
		}
	}

	// Triangles
	int c = 0;
	int vertexArrayLength = ny * nx;
	while (c < vertexArrayLength - nx - 1)
	{
		if (c == 0 || (((c + 1) % nx != 0) && c <= vertexArrayLength - nx))
		{
			AddTriangle(c + nx + 1, c + nx, c);
			AddTriangle(c, c + 1, c + nx + 1);
		}
		c++;
	}
}

void Mesh::Draw()
{
	if (VAO == 0)
		CreateBuffers(true, true, true);
	assert(VAO != 0);

	if (updateBuffersNextDraw == true)
		UpdateBuffers(true, true, true);

	glBindVertexArray(VAO);
	if (indices.size() > 0)
		glDrawElements(primitiveDrawn, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(primitiveDrawn, 0, (GLsizei)vertices.size());
}

void Mesh::Draw(const CameraOrbiter& orbiter)
{
	GLuint m_program = shader.GetProgram();
	assert(m_program != 0);
	glUseProgram(m_program);

	Transform trs = gameObject->GetObjectToWorldMatrix();
	Transform mvp = orbiter.Projection((float)orbiter.FrameWidth(), (float)orbiter.FrameHeight(), 45.0f) * (orbiter.View() * trs);
	Vector3 camPos = orbiter.Position();

	shader.UniformTransform("trsMatrix", trs);
	shader.UniformTransform("mvpMatrix", mvp);
	shader.UniformVec3("camPos", camPos);

	shader.UniformColor("diffuseColor", material.diffuse);
	shader.UniformFloat("shininess", material.shininess);
	shader.UniformInt("renderMode", renderMode);

	shader.UniformTexture("texture0", 0, texture0);

	if (renderMode == WireframeMode)
		primitiveDrawn = GL_LINES;
	else
		primitiveDrawn = GL_TRIANGLES;

	Draw();
}

void Mesh::WriteMesh(const char* filename) 
{
	cout << indices.size();
	cout << "Saving obj file " << endl;
	ofstream objfile;
	objfile.open(filename);
	objfile << "#Test COMMENT.\n";
	for (int i = 0; i < vertices.size(); i++)
		objfile << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << "\n";
	for (int j = 0; j < texcoords.size(); j++)
		objfile << "vt " << texcoords[j].x << " " << texcoords[j].y << "\n";
	for (int k = 0; k < normals.size() ; k++)
		objfile << "vn " << normals[k].x << " " << normals[k].y << " " << normals[k].z << "\n";
	for (int i = 0; i < indices.size(); i += 3)
		objfile << "f " << indices[i] + 1 << "/" << indices[i + 1] + 1 <<"/" << indices[i + 2] + 1 << "\n";
	
	bool has_texcoords = (texcoords.size() > 0);
	bool has_normals = (normals.size() > 0);
	bool has_indices = (indices.size() > 0);
	unsigned int n = has_indices ? (unsigned int)indices.size() : (unsigned int)vertices.size();
	for (unsigned int i = 0; i + 2 < n; i += 3)
	{
		objfile << "f ";
		for (unsigned int k = 0; k < 3; k++)
		{
			unsigned int id = has_indices ? indices[i + k] + 1 : i + k + 1;
			objfile << id;
			if (has_texcoords && has_normals)
				objfile << "/" << id << "/" << id << " ";
			else if (has_texcoords)
				objfile << "/" << id << " ";
			else if (has_normals)
				objfile << "//" << id << " ";
		}
		objfile << "\n";
	}
	objfile.close();
}

void Mesh::ReadMesh(const char* filename)
{
	FILE *in;
	errno_t err;
	if ((err = fopen_s(&in, filename, "r")) != 0)
	{
		std::cout << "Error loading mesh - aborting" << std::endl;
		return;
	}

	std::vector<Vector3> vert;
	std::vector<Vector2> tex;
	std::vector<Vector3> norm;

	std::vector<int> idp;
	std::vector<int> idt;
	std::vector<int> idn;

	char line_buffer[1024];
	bool error = true;
	for (;;)
	{
		if (fgets(line_buffer, sizeof(line_buffer), in) == NULL)
		{
			error = false;
			break;
		}

		line_buffer[sizeof(line_buffer) - 1] = 0;

		char *line = line_buffer;
		while (*line && isspace(*line))
			line++;

		if (line[0] == 'v')
		{
			float x, y, z;
			if (line[1] == ' ')          // position x y z
			{
				if (sscanf_s(line, "v %f %f %f", &x, &y, &z) != 3)
					break;
				vert.push_back(Vector3(x, y, z));
			}
			else if (line[1] == 'n')     // normal x y z
			{
				if (sscanf_s(line, "vn %f %f %f", &x, &y, &z) != 3)
					break;
				norm.push_back(Vector3(x, y, z));
			}
			else if (line[1] == 't')     // texcoord x y
			{
				if (sscanf_s(line, "vt %f %f", &x, &y) != 2)
					break;
				tex.push_back(Vector2(x, y));
			}
		}

		else if (line[0] == 'f')
		{
			idp.clear();
			idt.clear();
			idn.clear();

			int next;
			for (line = line + 1; ; line = line + next)
			{
				idp.push_back(0);
				idt.push_back(0);
				idn.push_back(0);

				next = 0;
				if (sscanf_s(line, " %d/%d/%d %n", &idp.back(), &idt.back(), &idn.back(), &next) == 3)
					continue;
				else if (sscanf_s(line, " %d/%d %n", &idp.back(), &idt.back(), &next) == 2)
					continue;
				else if (sscanf_s(line, " %d//%d %n", &idp.back(), &idn.back(), &next) == 2)
					continue;
				else if (sscanf_s(line, " %d %n", &idp.back(), &next) == 1)
					continue;
				else if (next == 0)
					break;
			}

			for (int v = 2; v + 1 < (int)idp.size(); v++)
			{
				int idv[3] = { 0, v - 1, v };
				for (int i = 0; i < 3; i++)
				{
					int k = idv[i];
					int p = (idp[k] < 0) ? (int)vert.size() + idp[k] : idp[k] - 1;
					int t = (idt[k] < 0) ? (int)tex.size() + idt[k] : idt[k] - 1;
					int n = (idn[k] < 0) ? (int)norm.size() + idn[k] : idn[k] - 1;

					if (p < 0) break;
					if (t >= 0) AddTexcoord(tex[t]);
					if (n >= 0) AddNormal(norm[n]);
					AddVertex(vert[p]);
				}
			}
		}
	}

	fclose(in);

	if (error)
		printf("loading mesh '%s'...\n[error]\n%s\n\n", filename, line_buffer);
}

void Mesh::SetTexture(const char* filename, const GLenum& texel_type)
{
	ImageData temp;
	temp.ReadImageData(filename);
	texture0 = MakeTexture(0, temp, texel_type);
}

GLuint Mesh::MakeTexture(const int unit, const ImageData& im, const GLenum& texel_type)
{
	if (im.data.empty())
		return 0;

	// cree la texture openGL
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);

	// fixe les parametres de filtrage par defaut
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLenum format;
	switch (im.channels)
	{
	case 1: format = GL_RED; break;
	case 2: format = GL_RG; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: format = GL_RGBA;
	}

	GLenum type;
	switch (im.size)
	{
	case 1: type = GL_UNSIGNED_BYTE; break;
	case 4: type = GL_FLOAT; break;
	default: type = GL_UNSIGNED_BYTE;
	}

	// transfere les donnees dans la texture
	glTexImage2D(GL_TEXTURE_2D, 0,
		texel_type, im.width, im.height, 0,
		format, type, im.buffer());

	// prefiltre la texture
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}
