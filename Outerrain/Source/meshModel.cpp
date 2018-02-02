#include "meshModel.h"

#include<iostream>


MeshModel::MeshModel() : isDirty(false)
{
}

void MeshModel::AddVertex(const Vector3& v)
{
	vertices.push_back(v);
	isDirty = true;
}

void MeshModel::AddNormal(const Vector3& n)
{
	normals.push_back(n);
	isDirty = true;
}

void MeshModel::AddTriangle(const unsigned int a, const unsigned int b, const unsigned int c)
{
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
	isDirty = true;
}

void MeshModel::AddTexcoord(const Vector2& t)
{
	texcoords.push_back(t);
	isDirty = true;
}


Bounds MeshModel::GetBounds() const
{
	Bounds ret;
	if (vertices.size() < 1)
		return ret;
	ret.a = Vector3(vertices[0]);
	ret.b = ret.a;
	for (unsigned int i = 1; i < (unsigned int)vertices.size(); i++)
	{
		Vector3 p = vertices[i];
		ret.a = Vector3(std::min(ret.a.x, p.x), std::min(ret.a.y, p.y), std::min(ret.a.z, p.z));
		ret.b = Vector3(std::max(ret.b.x, p.x), std::max(ret.b.y, p.y), std::max(ret.b.z, p.z));
	}
	return ret;
}

bool MeshModel::LoadObj(const std::string& path)
{
	std::ifstream ifs(path, std::ios::in);
	if (ifs.is_open() == false)
	{
		std::cout << "Error loading mesh at path : " << path.c_str() << std::endl;
		std::cout << "Aborting" << std::endl;
		return false;
	}
	ClearBuffers();

	std::string line;
	while (std::getline(ifs, line))
	{
		char identifier = line[0];
		if (identifier == 'v')
		{
			Vector3 tmp;
			std::stringstream content(line.substr(2, line.size()));
			char lineType = line[1];
			if (lineType == ' ')		// Vertex
			{
				content >> tmp.x >> tmp.y >> tmp.z;
				vertices.push_back(tmp);
			}
			else if (lineType == 'n')	// Normal
			{
				content >> tmp.x >> tmp.y >> tmp.z;
				normals.push_back(tmp);
			}
			else if (lineType == 't')	// Texcoord
			{
				content >> tmp.x >> tmp.y;
				texcoords.push_back(Vector2(tmp.x, tmp.y));
			}
		}
		else if (identifier == 'f')
		{
			std::stringstream content(line.substr(2, line.size()));
			int index1 = 0, index2 = 0, index3 = 0;
			content << index1 << index2 << index3;
			indices.push_back(index1);
			indices.push_back(index2);
			indices.push_back(index3);
		}
	}
	ifs.close();
	isDirty = true;

	return true;
}

void MeshModel::ClearBuffers()
{
	vertices.clear();
	normals.clear();
	texcoords.clear();
	indices.clear();
	isDirty = true;
}

void MeshModel::PrintInfos()
{
	std::cout << "Printing mesh properties..." << std::endl;
	std::cout << "Vertice count : " << VerticeCount() << std::endl;
	std::cout << "Normal count : " << NormalCount() << std::endl;
	std::cout << "Texcoord count : " << TexcoordCount() << std::endl;
	std::cout << "Indices count : " << IndicesCount() << std::endl;
}
