#include "layerfield.h"
#include "random.h"

/*
This class represents a heightfield made of several layers : bedrock, sediments, water etc...
This model is useful when trying to do an ecosystem simulation and study events such as fires, wind, water, lighting strikes...
Several ones are implemented here, but not all. If you want to know more about this, you can read the following papers :
	- https://hal.archives-ouvertes.fr/hal-01518967/document
	- https://perso.liris.cnrs.fr/eric.galin/Articles/2017-sparse-vegetation-terrains.pdf
*/

using Random = effolkronium::random_static;

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox)
{
	bedrock = Scalarfield2D(nx, ny, bbox, 1.0);

	rocks = Scalarfield2D(nx, ny, bbox, 0.0);
	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox, float value)
{
	bedrock = Scalarfield2D(nx, ny, bbox, value);

	rocks = Scalarfield2D(nx, ny, bbox, 0.0);
	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
LayerField::LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox)
{
	bedrock = Scalarfield2D(filePath, blackAltitude, whiteAltitude, nx, ny, bbox);

	rocks = Scalarfield2D(nx, ny, bbox, 0.0);
	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
Vector3 LayerField::Vertex(int i, int j) const
{
	Vector3 ret = bedrock.Vertex(i, j);
	ret.y += sediments.Get(i, j);
	return ret;
}

/*
\brief
*/
int LayerField::ToIndex1D(int i, int j) const
{
	return i * bedrock.SizeX() + j;
}

/*
\brief
*/
Box2D LayerField::GetBox() const
{
	return bedrock.GetBox();
}

/*
\brief
*/
int LayerField::SizeX() const
{
	return bedrock.SizeX();
}

/*
\brief
*/
int LayerField::SizeY() const
{
	return bedrock.SizeY();
}

/*
\brief Todo
*/
float LayerField::Height(const Vector2& p) const 
{
	float ret = bedrock.GetValueBilinear(p);
	ret += sediments.GetValueBilinear(p);
	return ret;
}

/*
\brief
*/
void LayerField::LightingEventSimulate(float strength, int probability, int fireProbability)
{
	if (Random::get(0, 100) < probability)
	{
		float x = static_cast<float>(Random::get(-bedrock.SizeX(), bedrock.SizeX()));
		float y = static_cast<float>(Random::get(-bedrock.SizeY(), bedrock.SizeY()));
		LightingEvent(Vector2(x, y), strength, fireProbability);
	}
}

/*
\brief Simulate a lighting impact with a specified force. Transform bedrock into rocks and destroys vegetation.
Also has a chance of triggering a fire in the vegetation layer.
\param position lighting impact position
\param strength between [0, 1]
\param fireProbability [0, 100]
*/
void LayerField::LightingEvent(const Vector2& position, float strength, int fireProbability)
{
	Vector2i gridCoordinates = bedrock.ToIndex2D(position);
	int radius = 5;

	// Impact in point neighborhood
	for (int i = gridCoordinates.x - radius; i < gridCoordinates.x + radius; i++)
	{
		for (int j = gridCoordinates.y - radius; j < gridCoordinates.y + radius; j++)
		{
			// Discard if not in terrain
			if (!bedrock.Inside(i, j))
				continue;

			// Look for lowest neighbour to go to
			float maxZDiff = 0.0;
			int neiI = -1, neiJ = -1;
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					if ((k == 0 && l == 0) || bedrock.Inside(i + k, j + l) == false)
						continue;
					float z = bedrock.Get(i, j) - bedrock.Get(i + k, j + l);
					if (z > maxZDiff)
					{
						maxZDiff = z;
						neiI = i + k;
						neiJ = j + l;
					}
				}
			}
			if (neiI == -1)
				continue;

			// Impact strength depends on distance to impact point
			float pointImpactStrength = strength - (Magnitude(Vector2(i, j) - Vector2(gridCoordinates)) / radius);
			bedrock.Remove(i, j, pointImpactStrength);
			vegetation.Set(i, j, -1.0f); // -1.0f means dead vegetation.
			rocks.Add(neiI, neiJ, pointImpactStrength);
		}
	}

	// Fire event
	if (Random::get(0, 100) < fireProbability)
		FireEventSimulate(position);

	// Stabilize rock & sediment layers
	Stabilize();
}

/*
\brief
*/
void LayerField::FireEventSimulate(const Vector2& startPosition)
{

}

/*
\brief Todo
*/
void LayerField::Stabilize()
{
}

/*
\brief
*/
MeshModel* LayerField::GetHeightfieldMesh() const
{
	int ny = bedrock.SizeY();
	int nx = bedrock.SizeX();

	MeshModel* ret = new MeshModel();
	ValueField<Vector3> normals = ValueField<Vector3>(bedrock.SizeX(), bedrock.SizeY(), bedrock.GetBox(), Vector3(0));
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			Vector3 AB = (Vertex(i + 1, j) - Vertex(i, j));
			Vector3 AC = (Vertex(i + 1, j + 1) - Vertex(i, j));
			Vector3 normal = Normalize(-Cross(AB, AC));

			normals.Set(i, j, normals.Get(i, j) + normal);
			normals.Set(i + 1, j, normals.Get(i + 1, j) + normal);
			normals.Set(i + 1, j + 1, normals.Get(i + 1, j + 1) + normal);

			AB = AC;
			AC = (Vertex(i, j + 1) - Vertex(i, j));
			normal = Normalize(-Cross(AB, AC));

			normals.Set(i, j, normals.Get(i, j) + normal);
			normals.Set(i + 1, j + 1, normals.Get(i + 1, j + 1) + normal);
			normals.Set(i, j + 1, normals.Get(i, j + 1) + normal);
		}
	}
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			normals.Set(i, j, Normalize(normals.Get(i, j)));
	}

	// Vertices & Texcoords & Normals
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / ((float)nx - 1);
			float v = i / ((float)ny - 1);
			ret->AddVertex(Vertex(i, j));
			ret->AddTexcoord(Vector2(u, v));
			ret->AddNormal(normals.Get(i, j));
		}
	}

	// Triangles
	int c = 0;
	int vertexArrayLength = ny * nx;
	while (c < vertexArrayLength - nx - 1)
	{
		if (c == 0 || (((c + 1) % nx != 0) && c <= vertexArrayLength - nx))
		{
			ret->AddTriangle(c + nx + 1, c + nx, c);
			ret->AddTriangle(c, c + 1, c + nx + 1);
		}
		c++;
	}

	return ret;
}

/*
\brief
*/
std::vector<EcosystemInstance> LayerField::GetVegetationInstances() const
{
	std::vector<EcosystemInstance> ret;
	for (int i = 0; i < vegetation.SizeY(); i++)
	{
		for (int j = 0; j < vegetation.SizeX(); j++)
		{
			if (vegetation.Get(i, j) != 0)
				ret.push_back(EcosystemInstance(vegetation.Vertex(i, j), vegetation.Get(i, j), 1));
		}
	}
	return ret;
}

/*
\brief
*/
std::vector<EcosystemInstance> LayerField::GetRockInstances() const
{
	std::vector<EcosystemInstance> ret;
	for (int i = 0; i < rocks.SizeY(); i++)
	{
		for (int j = 0; j < rocks.SizeX(); j++)
		{
			if (rocks.Get(i, j) != 0)
				ret.push_back(EcosystemInstance(rocks.Vertex(i, j), rocks.Get(i, j), 1));
		}
	}
	return ret;
}
