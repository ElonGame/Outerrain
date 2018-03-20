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
LayerField::LayerField(int nx, int ny, const Box2D& bbox) : Heightfield(nx, ny, bbox, 1.0)
{
	rocks = Scalarfield2D(nx, ny, bbox, 0.0);
	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox, float value) : Heightfield(nx, ny, bbox, value)
{
	rocks = Scalarfield2D(nx, ny, bbox, 0.0);
	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
LayerField::LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox) : Heightfield(filePath, blackAltitude, whiteAltitude, nx, ny, bbox)
{
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
	Vector3 ret = Heightfield::Vertex(i, j);
	ret.y += sediments.Get(i, j);
	return ret;
}

/*
\brief Todo
*/
float LayerField::Height(const Vector2& p) const 
{
	float ret = GetValueBilinear(p);
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
		float x = static_cast<float>(Random::get(-SizeX(), SizeX()));
		float y = static_cast<float>(Random::get(-SizeY(), SizeY()));
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
	Vector2i gridCoordinates = ToIndex2D(position);
	int radius = 5;

	// Impact in point neighborhood
	for (int i = gridCoordinates.x - radius; i < gridCoordinates.x + radius; i++)
	{
		for (int j = gridCoordinates.y - radius; j < gridCoordinates.y + radius; j++)
		{
			// Discard if not in terrain
			if (!Inside(i, j))
				continue;

			// Look for lowest neighbour to go to
			float maxZDiff = 0.0;
			int neiI = -1, neiJ = -1;
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					if ((k == 0 && l == 0) || Inside(i + k, j + l) == false)
						continue;
					float z = Get(i, j) - Get(i + k, j + l);
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
			Remove(i, j, pointImpactStrength);
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
MeshSetRenderer* LayerField::GetVegetationInstances() const
{
	MeshSetRenderer* ret = new MeshSetRenderer(new Mesh("Data/Objs/cube.obj"));
	
	return ret;
}

/*
\brief
*/
MeshSetRenderer* LayerField::GetRockInstances() const
{
	MeshSetRenderer* ret = new MeshSetRenderer(new Mesh("Data/Objs/cube.obj"));
	ret->SetMaterial(Material::DefaultDiffuseMat);
	for (int i = 0; i < rocks.SizeY(); i++)
	{
		for (int j = 0; j < rocks.SizeX(); j++)
		{
			float value = rocks.Get(i, j);
			if (value > 0.0f)
			{
				Frame newFrame;
				newFrame.SetPosition(Vertex(i, j));
				newFrame.SetScale(Vector3(1.0f));
				ret->AddFrame(newFrame);
			}
		}
	}
	return ret;
}
