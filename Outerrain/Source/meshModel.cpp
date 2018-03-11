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

void MeshModel::ReplaceVertices(const std::vector<Vector3>& newVertices)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices[i] = newVertices[i];
	isDirty = true;
}

void MeshModel::ReplaceNormals(const std::vector<Vector3>& newNormals)
{
	for (unsigned int i = 0; i < normals.size(); i++)
		normals[i] = newNormals[i];
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


// Heightfield Mesh Model
#include "heightfield.h"

HeightfieldMeshModel::HeightfieldMeshModel(Heightfield* h) : hf(h)
{
	// Vertices
	int nx = hf->SizeX();
	int ny = hf->SizeY();
	float nxMinusOne = (float)hf->SizeX() - 1;
	float nyMinusOne = (float)hf->SizeY() - 1;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / nxMinusOne;
			float v = i / nyMinusOne;
			AddVertex(hf->Vertex(i, j));
			AddTexcoord(Vector2(u, v));
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

	UpdateMeshBuffers();
}

void HeightfieldMeshModel::UpdateMeshBuffers()
{
	int nx = hf->SizeX();
	int ny = hf->SizeY();

	// Update vertex height
	for (int i = 0; i< ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			int index = hf->ToIndex1D(i, j);
			vertices[index].y = hf->Get(index);
		}
	}

	// Update normals
	normals.clear();
	ValueField<Vector3> normalsfield = ValueField<Vector3>(nx, ny, hf->GetBox(), Vector3(0));
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			Vector3 AB = (hf->Vertex(i + 1, j) - hf->Vertex(i, j));
			Vector3 AC = (hf->Vertex(i + 1, j + 1) - hf->Vertex(i, j));
			Vector3 normal = Normalize(-Cross(AB, AC));

			normalsfield.Set(i, j, normalsfield.Get(i, j) + normal);
			normalsfield.Set(i + 1, j, normalsfield.Get(i + 1, j) + normal);
			normalsfield.Set(i + 1, j + 1, normalsfield.Get(i + 1, j + 1) + normal);

			AB = AC;
			AC = (hf->Vertex(i, j + 1) - hf->Vertex(i, j));
			normal = Normalize(-Cross(AB, AC));

			normalsfield.Set(i, j, normalsfield.Get(i, j) + normal);
			normalsfield.Set(i + 1, j + 1, normalsfield.Get(i + 1, j + 1) + normal);
			normalsfield.Set(i, j + 1, normalsfield.Get(i, j + 1) + normal);
		}
	}
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			AddNormal(Normalize(normalsfield.Get(i, j)));
	}
}