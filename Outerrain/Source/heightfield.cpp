#include "heightfield.h"
#include "vec.h"
#include "fractal.h"
#include "mathUtils.h"
#include "random.h"

#include <iostream>
#include <numeric>
#include <deque>
#include <queue>
#include <array>

using Random = effolkronium::random_static;

/*
\brief Main Class for representing 2D HeightField.
Various functions and erosion processes are available (Wetness, Slope.. ; Thermal, Stream Power erosion...)
Can be rendered using GetMeshModel().
*/

/*
\brief Default constructor. Call Scalarfield2D default constructor.
*/
Heightfield::Heightfield() : Scalarfield2D()
{
}

/*
\brief Constructor
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
*/
Heightfield::Heightfield(int nx, int ny, const Box2D& bbox) : Scalarfield2D(nx, ny, bbox)
{
}

/*
\brief Constructor
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
\param value default value for the field
*/
Heightfield::Heightfield(int nx, int ny, const Box2D& bbox, float value) : Scalarfield2D(nx, ny, bbox, value)
{

}

/*
\brief Constructor from a file
\param file file path
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
*/
Heightfield::Heightfield(const std::string& file, float minAlt, float maxAlt, int nx, int ny, const Box2D& bbox) : Scalarfield2D(file, minAlt, maxAlt, nx, ny, bbox)
{
}

/*
\brief Constructor from a noise
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
\param amplitude noise amplitude
\param freq noise frequency
\param oct noise octave count
\param type fractal type. See enum.
*/
Heightfield::Heightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, FractalType type) : Scalarfield2D(nx, ny, bbox)
{
	InitFromNoise(n, amplitude, freq, oct, Vector3(0), type);
}

/*
\brief Constructor from a noise
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
\param amplitude noise amplitude
\param freq noise frequency
\param oct noise octave count
\param offset noise offset vector
\param type fractal type. See enum.
*/
Heightfield::Heightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type) : Scalarfield2D(nx, ny, bbox)
{
	InitFromNoise(n, amplitude, freq, oct, offset, type);
}

/*
\brief Constructor from TerrainSettings class for convenience and consistancy.
\param settings TerrainSettings for the heightfield.
*/
Heightfield::Heightfield(const TerrainSettings& settings) : Scalarfield2D(settings.nx, settings.ny, Box2D(settings.bottomLeft, settings.topRight))
{
	if (settings.terrainType == TerrainType::HeightFieldTerrain)
		ReadFromImage(settings.filePath.c_str(), settings.minAltitude, settings.maxAltitude);
	else if (settings.terrainType == TerrainType::NoiseFieldTerrain)
		InitFromNoise(*settings.noise, settings.amplitude, settings.frequency, settings.octaves, settings.offsetVector, settings.fractalType);
}

/*
\brief Internal method to init a heightfield with noise. Only called from the constructor.
\param n used noise
\param amplitude noise amplitude
\param freq noise frequency
\param oct noise octave count
\param offset noise offset translation
\param type noise fractal type.
*/
void Heightfield::InitFromNoise(const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type)
{
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			Vector3 p = Vertex(i, j);
			p = p + offset;
			float h = 0.0f;
			if (type == FractalType::fBm)
				h = Fractal::fBm(n, p, amplitude, freq, oct);
			else if (type == FractalType::Ridge)
				h = Fractal::RidgeNoise(n, p * freq, amplitude, freq, oct);
			else if (type == FractalType::MusgravefBm)
				h = (amplitude / 2.0) * Fractal::MusgravefBm(n, p * freq, 1.0f, 2.0f, oct);
			else if (type == FractalType::MusgraveHeteroTerrain)
				h = (amplitude / 2.0f) * (Fractal::MusgraveHeteroTerrain(n, p * freq, 1.0f, 2.0f, oct, 1.0f) * 0.5f - 0.5f);
			else if (type == FractalType::MusgraveHybridMultifractal)
				h = amplitude * Fractal::MusgraveHybridMultifractal(n, p * freq, 0.25f, 2.0f, oct, 0.7f);
			else if (type == FractalType::MusgraveRidgedMultifractal)
				h = amplitude * Fractal::MusgraveRidgedMultifractal(n, p * freq, 1.0f, 2.0f, oct, 1.0f, 2.0f);
			Set(i, j, h);
		}
	}
}

/*
\brief Destructor
*/
Heightfield::~Heightfield()
{
}

/*
\brief Perform a thermal erosion step with maximum amplitude defined by user. Based on http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.27.8939&rep=rep1&type=pdf.
\param amplitude maximum amount of matter moved from one point to another. Something between [0.05, 0.1] gives plausible results.
*/
void Heightfield::ThermalWeathering(float amplitude, float tanThresholdAngle)
{
	float cellDistX = CellSize().x;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float maxZDiff = 0.0f;
			int neiI, neiJ;
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

			if (maxZDiff / cellDistX > tanThresholdAngle)
			{
				Remove(i, j, amplitude);
				Add(neiI, neiJ, amplitude);
			}
		}
	}
}

/*
\brief Perform a stream power erosion step with maximum amplitude defined by user. Based on https://hal.inria.fr/hal-01262376/document.
This erosion called 'Fluvial' is based on Drainasge and Slope. One of the weakness of the stream power erosion is that it can create peaks
and generally unrealistic height sometimes. Therefore it can be improved by checking for slopes higher than 30� and not performing erosion.

\param amplitude maximum amount of matter eroded in one step. Something between [0.5, 1.0] gives plausible results.
*/
void Heightfield::StreamPowerErosion(float amplitude)
{
	Scalarfield2D SP = StreamPower();
	SP.NormalizeField();
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float oldH = Get(i, j);
			float newH = oldH - (SP.Get(i, j) * amplitude);
			Set(i, j, newH);
		}
	}
}

#include <iostream>
using namespace std;
/*
\brief
*/
void Heightfield::HydraulicErosion()
{
	Scalarfield2D droplets(nx, ny, box, 1.0f);
	Scalarfield2D sediments(nx, ny, box, 1.0f);

	const float Kd = 0.1f;
	const float Kc = 5.0f;
	const float Ks = 0.3f;

	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			std::array<float, 8> waterTransport, neighbourHeightDiff;
			waterTransport.fill(0);
			neighbourHeightDiff.fill(0.0f);
			int lowerVertexCount = 0;
			int index = 0;
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					if ((k == 0 && l == 0) || Inside(i + k, j + l) == false)
						continue;

					neighbourHeightDiff[index] = Get(i, j) - Get(i + k, j + l);
					if (neighbourHeightDiff[index] <= 0.0f)
					{
						index++;
						continue;
					}

					float a0 = droplets.Get(i, j);
					float a1 = droplets.Get(i + k, j + l) + Get(i + k, j + l);
					float a2 = a0 + Get(i, j);
					waterTransport[index] = Math::Min(a0, a2 - a1);
					lowerVertexCount++;
					index++;
				}
			}

			index = 0;
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					if ((k == 0 && l == 0) || Inside(i + k, j + l) == false)
						continue;
					if (neighbourHeightDiff[index] <= 0.0f)
					{
						index++;
						continue;
					}

					// Bug here for sure
					// Remake implementation based on http://hpcg.purdue.edu/bbenes/papers/Benes02WSCG.pdf
					// Instead of Musgrave, which is not very precise and misses some details.
					waterTransport[index] = waterTransport[index] * neighbourHeightDiff[index] / lowerVertexCount;
					waterTransport[index] = Math::Clamp(waterTransport[index], 0.0f, 1.0f);
					//cout << waterTransport[index] << endl;

					if (waterTransport[index] <= 0.0f)
					{
						Add(i, j, Kd * sediments.Get(i, j));
						sediments.Set(i, j, (1.0f - Kd) * sediments.Get(i, j));
					}
					else
					{
						droplets.Remove(i, j, waterTransport[index]);
						droplets.Add(i + k, j + l, waterTransport[index]);
						float Cs = Kc * waterTransport[index];
						float sedA = sediments.Get(i, j);

						if (sedA > Cs)
						{
							sediments.Add(i + k, j + l, Cs);
							Add(i, j, Kd * (sedA - Cs));
							sediments.Set(i, j, (1 - Kd) * (sedA - Cs));
						}
						else
						{
							sediments.Add(i + k, j + l, sedA + Ks * (Cs - sedA));
							Remove(i, j, Kd * (Cs - sedA));
							sediments.Set(i, j, 0.0f);
						}
					}

					index++;
				}
			}
		}
	}
}

/*
\brief Compute the Drainage Area field.
*/
Scalarfield2D Heightfield::DrainageArea() const
{
	// Sort all point by decreasing height
	std::deque<ScalarValue> points;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			points.push_back(ScalarValue(i, j, Get(i, j)));
	}
	std::sort(points.begin(), points.end(), [](ScalarValue p1, ScalarValue p2) { return p1.value > p2.value; });

	std::array<float, 8> slopes;
	std::array<Vector2i, 8> coords;
	Scalarfield2D DA = Scalarfield2D(nx, ny, box, 1.0);
	while (!points.empty())
	{
		ScalarValue p = points.front();
		points.pop_front();

		slopes.fill(0.0f);
		int i = p.x, j = p.y;
		float h = Get(i, j);
		int neighbourCount = 0;
		for (int k = -1; k <= 1; k++)
		{
			for (int l = -1; l <= 1; l++)
			{
				if ((k == 0 && l == 0) || Inside(i + k, j + l) == false)
					continue;
				// If current point as lower neighbour : compute slope to later distribute accordingly.
				float nH = Get(i + k, j + l);
				if (h > nH)
				{
					float dH = h - nH;
					if (k + l == -1 || k + l == 1)
						slopes[neighbourCount] = dH;
					else
						slopes[neighbourCount] = dH / sqrt(2.0f);

					coords[neighbourCount] = Vector2i(i + k, j + l);
					neighbourCount++;
				}
			}
		}

		// Distribute to those lower neighbours
		float sum = Math::Sum<float, 8>(slopes);
		for (int k = 0; k < neighbourCount; k++)
			DA.Set(coords[k], DA.Get(coords[k]) + DA.Get(i, j) * (slopes[k] / sum));
	}
	return DA;
}

/*
\brief Compute the slope field, ie Norm(Gradient(i, j)).
*/
Scalarfield2D Heightfield::Slope() const
{
	Scalarfield2D S = Scalarfield2D(nx, ny, box);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			S.Set(i, j, Magnitude(Gradient(i, j)));
	}
	return S;
}

/*
\brief Compute the Wetness Index Field.
*/
Scalarfield2D Heightfield::Wetness() const
{
	Scalarfield2D DA = DrainageArea();
	Scalarfield2D S = Slope();
	Scalarfield2D W = Scalarfield2D(nx, ny, box);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			W.Set(i, j, abs(log(DA.Get(i, j) / (1.0f + S.Get(i, j)))));
	}
	return W;
}

/*
\brief Compute the StreamPower field, as described by http://geosci.uchicago.edu/~kite/doc/Whipple_and_Tucker_1999.pdf.
*/
Scalarfield2D Heightfield::StreamPower() const
{
	Scalarfield2D DA = DrainageArea();
	Scalarfield2D S = Slope();
	Scalarfield2D SP = Scalarfield2D(nx, ny, box);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			SP.Set(i, j, sqrt(DA.Get(i, j)) * S.Get(i, j));
	}
	return SP;
}

/*
\brief Compute the 'Illumination' field, which is basically an approximation of Ambient Occlusion.
*/
Scalarfield2D Heightfield::Illumination() const
{
	const int rayCount = 32;		// Ray count for each world point
	const float epsilon = 0.01f;	// Ray start up offset
	const float K = Slope().Max();	// Lipschitz constant
	Scalarfield2D Illu = Scalarfield2D(nx, ny, box);
	Hit rayHit;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			Vector3 rayPos = Vertex(i, j) + Vector3(0.0, epsilon, 0.0);
			float h = Get(i, j);
			int intersectionCount = 0;

			for (int k = 0; k < rayCount; k++)
			{
				float angleH = (Random::get() % 360) * 0.0174533f;
				float angleV = Random::get() / static_cast<float>(Random::max());
				Vector3 rayDir = Vector3(cos(angleH), 0.0f, sin(angleH));
				rayDir = Slerp(rayDir, Vector3(0.0f, 1.0f, 0.0f), angleV);
				if (Intersect(Ray(rayPos, rayDir), rayHit, K) == true)
					intersectionCount++;
			}
			Illu.Set(i, j, 1.0f - (intersectionCount / static_cast<float>(rayCount)));
		}
	}
	return Illu;
}

/*
\brief Compute the intersection between a heightfield and a ray, using Sphere Tracing and a user defined Lipschitz constant.
\param ray
\param hit returned hit
\param K Lipschitz Constant
\return true of intersection occured, false otherwise
*/
bool Heightfield::Intersect(const Ray& ray, Hit& hit, float K) const
{
	Box bbox = GetBox().ToBox(Min(), Max());
	float a, b;
	if (!bbox.Intersect(ray, a, b))
		return false;

	float t = Math::Max(a + 0.01f, 0.0f);
	while (t < b)
	{
		Vector3 p = ray.At(t);
		float z = GetValueBilinear(Vector2(p.x, p.y));
		float h = p[2] - z;
		if (h < 0.01f)
		{
			hit.position = Vector3(p.x, p.y, z);
			hit.normal = Vector3(0);
			return true;
		}
		t += Math::Max(h / K, 0.1f);
	}
	return false;
}

/*
\brief Compute the intersection between a heightfield and a ray, using Sphere Tracing.
Lipschitz constant is defined as the maximum slope
\param ray
\param hit returned hit
\return true of intersection occured, false otherwise
*/
bool Heightfield::Intersect(const Ray& ray, Hit& hit) const
{
	return Intersect(ray, hit, Slope().Max());
}

/*
\brief Compute the intersection between a heightfield and a ray, using Sphere Tracing.
Lipschitz constant is defined as the maximum slope
\param origin ray origin
\param direction ray direction
\param hitPos returned hit position
\param hitNormal returned hit normal
\return true of intersection occured, false otherwise
*/
bool Heightfield::Intersect(const Vector3& origin, const Vector3& direction, Vector3& hitPos, Vector3& hitNormal) const
{
	Hit hit;
	bool res = Intersect(Ray(origin, direction), hit, Slope().Max());
	hitPos = hit.position;
	hitNormal = hit.normal;
	return res;
}

/*
\brief Compute the heightfield mesh for rendering
*/
Mesh* Heightfield::GetMeshModel() const
{
	Mesh* ret = new Mesh();
	ValueField<Vector3> normals = ValueField<Vector3>(nx, ny, box, Vector3(0));
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
