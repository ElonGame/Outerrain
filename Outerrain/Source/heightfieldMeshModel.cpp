#include "heightfieldMeshModel.h"
#include "heightfield.h"
#include "layerfield.h"

HeightfieldMesh::HeightfieldMesh(Heightfield* h) : hf(h)
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

void HeightfieldMesh::UpdateMeshBuffers()
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
