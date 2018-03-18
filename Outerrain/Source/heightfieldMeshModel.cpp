#include "heightfieldMeshModel.h"
#include "heightfield.h"
#include "layerfield.h"

HeightfieldMeshModel::HeightfieldMeshModel(Heightfield* h) : hf(h), lf(nullptr)
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

HeightfieldMeshModel::HeightfieldMeshModel(LayerField* h) : hf(nullptr), lf(h)
{
	// Vertices
	int nx = lf->SizeX();
	int ny = lf->SizeY();
	float nxMinusOne = (float)lf->SizeX() - 1;
	float nyMinusOne = (float)lf->SizeY() - 1;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / nxMinusOne;
			float v = i / nyMinusOne;
			AddVertex(lf->Vertex(i, j));
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
	if (hf)
		UpdateMeshBuffersHeightfield();
	else
		UpdateMeshBuffersLayerField();
}

void HeightfieldMeshModel::UpdateMeshBuffersHeightfield()
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

void HeightfieldMeshModel::UpdateMeshBuffersLayerField()
{
	int nx = lf->SizeX();
	int ny = lf->SizeY();

	// Update vertex height
	for (int i = 0; i< ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			int index = lf->ToIndex1D(i, j);
			vertices[index].y = lf->Vertex(i, j).y;
		}
	}

	// Update normals
	normals.clear();
	ValueField<Vector3> normalsfield = ValueField<Vector3>(nx, ny, lf->GetBox(), Vector3(0));
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			Vector3 AB = (lf->Vertex(i + 1, j) - lf->Vertex(i, j));
			Vector3 AC = (lf->Vertex(i + 1, j + 1) - lf->Vertex(i, j));
			Vector3 normal = Normalize(-Cross(AB, AC));

			normalsfield.Set(i, j, normalsfield.Get(i, j) + normal);
			normalsfield.Set(i + 1, j, normalsfield.Get(i + 1, j) + normal);
			normalsfield.Set(i + 1, j + 1, normalsfield.Get(i + 1, j + 1) + normal);

			AB = AC;
			AC = (lf->Vertex(i, j + 1) - lf->Vertex(i, j));
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
