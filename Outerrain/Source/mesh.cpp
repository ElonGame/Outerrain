#include "mesh.h"

#include<iostream>

#ifdef _WIN32
#define SSCANF sscanf_s
#define FOPEN fopen_s
#else
#define SSCANF sscanf
#define FOPEN fopen
#endif

Mesh::Mesh() : isDirty(false)
{
}

Mesh::Mesh(const std::string& path) : isDirty(false)
{
	LoadObj(path);
}

void Mesh::AddVertex(const Vector3& v)
{
	vertices.push_back(v);
	isDirty = true;
}

void Mesh::AddNormal(const Vector3& n)
{
	normals.push_back(n);
	isDirty = true;
}

void Mesh::AddTriangle(unsigned int a, unsigned int b, unsigned int c)
{
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
	isDirty = true;
}

void Mesh::AddTexcoord(const Vector2& t)
{
	texcoords.push_back(t);
	isDirty = true;
}

Box Mesh::GetBounds() const
{
	Box ret = Box(Vector3(0), 1.0);
	if (vertices.size() < 1)
		return ret;
	ret = Box(vertices[0], vertices[0]);
	for (unsigned int i = 1; i < vertices.size(); i++)
	{
		Vector3 p = vertices[i];
		ret[0] = Vector3(Math::Min(ret[0].x, p.x), Math::Min(ret[0].y, p.y), Math::Min(ret[0].z, p.z));
		ret[1] = Vector3(Math::Max(ret[1].x, p.x), Math::Max(ret[1].y, p.y), Math::Max(ret[1].z, p.z));
	}
	return ret;
}

bool Mesh::LoadObj(const std::string& path)
{
	FILE* in;
#if _WIN32
	FOPEN(&in, path.c_str(), "r");
#else
	in = FOPEN(path.c_str(), "r");
#endif
	if (in == NULL)
	{
		std::cout << "Error loading mesh - aborting" << std::endl;
		return false;
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
				if (SSCANF(line, "v %f %f %f", &x, &y, &z) != 3)
					break;
				vert.push_back(Vector3(x, y, z));
			}
			else if (line[1] == 'n')     // normal x y z
			{
				if (SSCANF(line, "vn %f %f %f", &x, &y, &z) != 3)
					break;
				norm.push_back(Vector3(x, y, z));
			}
			else if (line[1] == 't')     // texcoord x y
			{
				if (SSCANF(line, "vt %f %f", &x, &y) != 2)
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
				if (SSCANF(line, " %d/%d/%d %n", &idp.back(), &idt.back(), &idn.back(), &next) == 3)
					continue;
				else if (SSCANF(line, " %d/%d %n", &idp.back(), &idt.back(), &next) == 2)
					continue;
				else if (SSCANF(line, " %d//%d %n", &idp.back(), &idn.back(), &next) == 2)
					continue;
				else if (SSCANF(line, " %d %n", &idp.back(), &next) == 1)
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
	{
		printf("loading mesh '%s'...\n[error]\n%s\n\n", path.c_str(), line_buffer);
		return false;
	}
	return true;
}

void Mesh::ClearBuffers()
{
	vertices.clear();
	normals.clear();
	texcoords.clear();
	indices.clear();
	isDirty = true;
}

void Mesh::PrintInfos()
{
	std::cout << "Printing mesh properties..." << std::endl;
	std::cout << "Vertice count : " << VertexCount() << std::endl;
	std::cout << "Normal count : " << NormalCount() << std::endl;
	std::cout << "Texcoord count : " << TexcoordCount() << std::endl;
	std::cout << "Indices count : " << TriangleCount() << std::endl;
}
