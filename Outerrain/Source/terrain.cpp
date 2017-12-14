#include <algorithm>
#include <math.h>
#include <deque>
#include <queue>
#include <array>
#include "terrain.h"
#include "vec.h"
#include "perlin.h"
#include "image.h"
#include "vegetationObject.h"
#include "gameobject.h"


static bool compareHeight(Point u, Point v)
{
	return (u.height > v.height);
}


/* Terrain2D */
Terrain2D::Terrain2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	heightField = ScalarField2D(nx, ny, bottomLeft, topRight);
	normalField = ValueField<Vector3>(nx, ny, bottomLeft, topRight);
}

void Terrain2D::InitFromNoise(int blackAltitude, int whiteAltitude)
{
	float whiteFloat = static_cast<float>(whiteAltitude);
	float blackFloat = static_cast<float>(blackAltitude);
	float diff = whiteFloat - blackFloat;
	PerlinNoise noise;
	float multiplier = 307.06571f;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float iF = ((float)(i + 1)) * multiplier;
			float jF = ((float)(j + 1)) * multiplier;
			float v = blackFloat + noise.Fbm(Vector2(iF, jF), (float)diff, 0.00001f, 8);
			heightField.Set(i, j, v);
		}
	}
	ComputeNormalField();
}

void Terrain2D::InitFromFile(const char* path, int blackAltitude, int whiteAltitude)
{
	heightField.ReadImageGrayscale(path, blackAltitude, whiteAltitude);
	ComputeNormalField();
}

float Terrain2D::Height(const Vector2& p) const
{
	return heightField.GetValueBilinear(p);
}

Vector3 Terrain2D::Normal(int i, int j) const
{
	return normalField.Get(i, j);
}

Vector3 Terrain2D::Vertex(int i, int j) const
{
	float x = bottomLeft.x + j * (topRight.x - bottomLeft.x) / (nx - 1);
	float y = heightField.Get(i, j);
	float z = bottomLeft.y + i * (topRight.y - bottomLeft.y) / (ny - 1);
	return Vector3(x, y, z);
}

Mesh* Terrain2D::GetMesh() const
{
	Mesh* ret = new Mesh();
	ret->CalculateFromTerrain2D(*this);
	return ret;
}

void Terrain2D::SetHeight(int i, int j, float v)
{
	heightField.Set(i, j, v);
}

void Terrain2D::ComputeNormalField()
{
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			Vector3 AB = (Vertex(i + 1, j) - Vertex(i, j));
			Vector3 AC = (Vertex(i + 1, j + 1) - Vertex(i, j));
			Vector3 normal = Normalize(Cross(AB, AC));

			normalField.Set(i, j, normalField.Get(i, j) + normal);
			normalField.Set(i + 1, j, normalField.Get(i + 1, j) + normal);
			normalField.Set(i + 1, j + 1, normalField.Get(i + 1, j + 1) + normal);

			AB = AC;
			AC = (Vertex(i, j + 1) - Vertex(i, j));
			normal = Normalize(Cross(AB, AC));

			normalField.Set(i, j, normalField.Get(i, j) + normal);
			normalField.Set(i + 1, j + 1, normalField.Get(i + 1, j + 1) + normal);
			normalField.Set(i, j + 1, normalField.Get(i, j + 1) + normal);
		}
	}
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			normalField.Set(i, j, Normalize(normalField.Get(i, j)));
	}
}

float Terrain2D::NormalizedHeight(const Vector2& p) const
{
	float h = Height(p);
	return h / heightField.MaxValue();
}

/* Erosion */
void Terrain2D::StreamPowerErosion(int iteration, float maxAmplitude)
{
	ScalarField2D streamPower = StreamPowerField();
	for (int k = 0; k < iteration; k++)
	{
		for (int i = 0; i < ny; i++)
		{
			for (int j = 0; j < nx; j++)
			{
				float newHeight = heightField.Get(i, j) - (streamPower.Get(i, j) * maxAmplitude);
				heightField.Set(i, j, newHeight);
			}
		}
	}
}

/* Useful Fields */
ScalarField2D Terrain2D::SlopeField() const
{
	// Slope(i, j) : ||Gradient(i, j)||
	ScalarField2D slopeField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			slopeField.Set(i, j, Magnitude(heightField.Gradient(i, j)));
	}
	return slopeField;
}

int Terrain2D::Distribute(Point p, std::array<Point, 8>& neighbours, std::array<float, 8>& height, std::array<float, 8>& slope) const
{
	// @TODO Diviser currentSlope et currentHeight par c.x en horizontal et c.y en vertical et norme en diagonale
	int i = p.x, j = p.y;
	int counter = 0;
	float currentSlope = 0.0f;
	float currentHeight = 0.0f;
	float pointHeight = heightField.Get(i, j);

	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{
			if (k == 0 || l == 0 || heightField.InsideVertex(i + k, j + l) == false)
				continue;

			float neighHeight = heightField.Get(i + k, j + l);
			if (pointHeight > neighHeight)
			{
				currentHeight = pointHeight - neighHeight;
				if (k + l == -1 || k + l == 1)
					currentSlope = currentHeight;
				else
					currentSlope = currentHeight / sqrt(2.0f);

				neighbours[counter] = Point(i + k, j + l, 0.0f);
				height[counter] = currentHeight;
				slope[counter] = currentSlope;
				counter++;
			}
		}
	}
	return counter;
}

ScalarField2D Terrain2D::DrainageField() const
{
	std::deque<Point> points;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			points.push_back(Point(i, j, heightField.Get(i, j)));
	}
	std::sort(points.begin(), points.end(), compareHeight);

	ScalarField2D drainage = ScalarField2D(nx, ny, bottomLeft, topRight, 1.0);
	while (!points.empty())
	{
		Point p = points.front();
		points.pop_front();

		int i = p.x, j = p.y;
		std::array<Point, 8> neighbours;
		std::array<float, 8> slope;
		std::array<float, 8> height;
		int n = Distribute(p, neighbours, height, slope);

		float sum = 0.0;
		for (int k = 0; k < n; k++)
			sum += slope[k];

		for (int k = 0; k < n; k++)
		{
			int l = neighbours[k].x, m = neighbours[k].y;
			drainage.Set(l, m, drainage.Get(l, m) + drainage.Get(i, j) * (slope[k] / sum));
		}
	}
	return drainage;
}

ScalarField2D Terrain2D::DrainageSqrtField() const
{
	ScalarField2D drainageField = DrainageField();
	ScalarField2D sqrtDrainageField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			sqrtDrainageField.Set(i, j, sqrt(drainageField.Get(i, j)));
	}
	return sqrtDrainageField;
}

ScalarField2D Terrain2D::WetnessField() const
{
	ScalarField2D drainageField = DrainageField();
	ScalarField2D slopeField = SlopeField();
	ScalarField2D wetnessField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			wetnessField.Set(i, j, log(drainageField.Get(i, j) / (1.0f + slopeField.Get(i, j))));
	}
	return wetnessField;
}

ScalarField2D Terrain2D::StreamPowerField() const
{
	ScalarField2D drainageField = DrainageField();
	ScalarField2D slopeField = SlopeField();
	ScalarField2D streamPowerField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			streamPowerField.Set(i, j, sqrt(drainageField.Get(i, j)) * slopeField.Get(i, j));
	}
	return streamPowerField;
}

ScalarField2D Terrain2D::AccessibilityField() const
{
	float epsilon = 0.01f;
	float maxSlope = SlopeField().MaxValue();
	ScalarField2D accessibilityField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			Vector3 p = Vertex(i, j) + Vector3(0.0, epsilon, 0.0);
			float h = heightField.Get(i, j);

			int numbers = 10;
			int intersect = 0;

			for (int k = 0; k < numbers; k++)
			{
				float step = 0.0f;
				Vector3 ray = p;
				float angleH = (rand() % 360) * 0.0174533f;
				float angleV = rand() / static_cast<float>(RAND_MAX);
				Vector3 direction = Vector3(cos(angleH), 0.0f, sin(angleH));
				direction = Slerp(direction, Vector3(0.0f, 1.0f, 0.0f), angleV);
				for (int l = 0; l < 32; l++)
				{
					ray = p + direction * step;

					Vector2 terrainRay = Vector2(ray.x, ray.z);
					if (heightField.IsInsideField(terrainRay) == false)
						break;

					float terrainHeight = Height(terrainRay);
					float deltaY = ray.y - terrainHeight;

					if (deltaY < 0.0f)
					{
						intersect++;
						break;
					}
					step = deltaY / maxSlope;
				}
			}
			float illumination = 1.0f - (intersect / static_cast<float>(numbers));
			accessibilityField.Set(i, j, illumination);
		}
	}
	return accessibilityField;
}

std::vector<Vector3> Terrain2D::GetAllVertices() const
{
	std::vector<Vector3> ret;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			ret.push_back(Vertex(i, j));
	}
	return ret;
}


/* LayerTerrain2D */
LayerTerrain2D::LayerTerrain2D(int nx, int ny, Vector2 a, Vector2 b)
	: nx(nx), ny(ny), a(a), b(b)
{
	sand = ScalarField2D(nx, ny, a, b);
	bedrock = ScalarField2D(nx, ny, a, b);
}

void LayerTerrain2D::InitFromFile(const char* file, int blackAltitude, int whiteAltitude, float sandValue)
{
	bedrock.ReadImageGrayscale(file, blackAltitude, whiteAltitude);
	sand.Fill(sandValue);
}

Vector3 LayerTerrain2D::Vertex(int i, int j) const
{
	float x = a.x + j * (b.x - a.x) / (nx - 1);
	float y = Height(i, j);
	float z = a.y + i * (b.y - a.y) / (ny - 1);
	return Vector3(x, y, z);
}

float LayerTerrain2D::Height(int i, int j) const
{
	return BeckrockValue(i, j) + SandValue(i, j);
}

float LayerTerrain2D::BeckrockValue(int i, int j) const
{
	return bedrock.Get(i, j);
}

float LayerTerrain2D::SandValue(int i, int j) const
{
	return sand.Get(i, j);
}

void LayerTerrain2D::ThermalErosion(int stepCount)
{
	// Constants
	float K = 0.1f;		 // Stress factor
	float Alpha = 42.0f; // Threshold Angle for stability (40 +- 5)
	float tanAlpha = tan(Alpha);

	for (int a = 0; a < stepCount; a++)
	{
		std::queue<int> instables;
		std::queue<float> matter; // quantite de matiere qui est transportee
		for (int i = 0; i < bedrock.SizeX(); i++)
		{
			for (int j = 0; j < bedrock.SizeY(); j++)
			{
				// On calcule le delta H 
				// Pour ca on fait le max des delta H des voisins de I
				// float dH = bedrock.At(i,j) - bedrock.At(i, j + 1);
				float dH1 = 0.0, dH2 = 0.0, dH3 = 0.0, dH4 = 0.0;
				if (j < bedrock.SizeY() - 1)
					dH1 = bedrock.Get(i, j) - bedrock.Get(i, j + 1);
				if (i < bedrock.SizeX() - 1)
					dH2 = bedrock.Get(i, j) - bedrock.Get(i + 1, j);
				if (i > 0)
					dH3 = bedrock.Get(i, j) - bedrock.Get(i - 1, j);
				if (j > 0)
					dH4 = bedrock.Get(i, j) - bedrock.Get(i, j - 1);
				float dH = std::max(dH1, std::max(dH2, std::max(dH3, dH4)));

				// If dH < 0, it means that no neighbours are lower than us
				// So we discard the point
				if (dH <= 0.0)
					continue;

				// Instability criteria
				// @Todo : Refactor delta in a function (sqrt(2) for diagonal neighbours)
				float delta = abs(bedrock.TopRight().x - bedrock.BottomLeft().x) / bedrock.SizeX();
				if (abs(dH) / delta > tanAlpha)
				{
					// AddToQueue INSTABLE
					instables.push(bedrock.Index(i, j));

					// Pour calculer stress, c'est a dire quantite de matiere a eroder :
					// Stress = k * dH
					matter.push(abs(dH * K));
				}
			}
		}
		while (instables.empty() == false)
		{
			int index = instables.front();
			instables.pop();
			float eps = matter.front();
			matter.pop();

			int indexLVoisin = bedrock.LowestNeighbour(index);
			bedrock.Set(index, bedrock.Get(index) - eps);
			sand.Set(indexLVoisin, sand.Get(indexLVoisin) + eps);

			//TODO : Push(Voisin) ?
		}
	}
}

Mesh* LayerTerrain2D::GetMesh() const
{
	// Final terrain
	Terrain2D terrain = Terrain2D(nx, ny, a, b);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			terrain.SetHeight(i, j, Height(i, j));
	}
	terrain.ComputeNormalField();
	return terrain.GetMesh();
}

std::vector<Vector3> LayerTerrain2D::GetAllVertices() const
{
	std::vector<Vector3> ret;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			ret.push_back(Vertex(i, j));
	}
	return ret;
}


/* VegetationTerrain */
VegetationTerrain::VegetationTerrain(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: Terrain2D(nx, ny, bottomLeft, topRight),
	vegetationDensityField(nx, ny, bottomLeft, topRight)
{
}

void VegetationTerrain::ComputeVegetationDensities()
{
	ScalarField2D slopeField = SlopeField();
	VegetationObject vegObj;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float slope = slopeField.Get(i, j);
			vegetationDensityField.Set(i, j, vegObj.SlopeDensityFactor(slope));
		}
	}
}

std::vector<GameObject*> VegetationTerrain::GetTreeObjects() const
{
	VegetationObject veg;
	veg.SetRadius(3.0f);
	float tileSize = veg.GetRadius() * 10.0f;
	std::vector<Vector2> points = GetRandomDistribution(veg.GetRadius(), tileSize, 100);

	int maxTreeCount = 1000;
	int treeCount = 0;

	int tileCountX = static_cast<int>(((topRight.x - bottomLeft.x) / tileSize + 1));
	int tileCountY = static_cast<int>(((topRight.y - bottomLeft.y) / tileSize + 1));

	std::vector<GameObject*> vegObjects;
	for (int i = 0; i < tileCountY; i++)
	{
		for (int j = 0; j < tileCountX; j++)
		{
			for (int x = 0; x < points.size(); x++)
			{
				Vector2 point = bottomLeft
					+ Vector2(tileSize, 0) * static_cast<float>(j)
					+ Vector2(0, tileSize) * static_cast<float>(i)
					+ points[x];
				if (vegetationDensityField.IsInsideField(point) == true)
				{
					float density = vegetationDensityField.GetValueBilinear(point);
					float p = rand() / static_cast<float>(RAND_MAX);
					if (p < density)
					{
						GameObject* vegObj = veg.GetGameObject();
						Vector3 pos = Vector3(point.x, Height(point), point.y);
						vegObj->SetPosition(pos);
						vegObjects.push_back(vegObj);
						treeCount++;
					}
				}
				if (treeCount >= maxTreeCount)
					return vegObjects;
			}
		}
	}
	return vegObjects;
}

std::vector<Vector2> VegetationTerrain::GetRandomDistribution(float objRadius, float tileSize, int maxTries) const
{
	std::vector<Vector2> res;
	for (int i = 0; i < maxTries; i++)
	{
		float randX = rand() / static_cast<float>(RAND_MAX);
		float randY = rand() / static_cast<float>(RAND_MAX);
		Vector2 point = Vector2(randX * tileSize, randY * tileSize);

		bool canAdd = true;
		for (int j = 0; j < res.size(); j++)
		{
			if (Magnitude(point - res[j]) <= objRadius)
			{
				canAdd = false;
				break;
			}
		}
		if (canAdd == true)
			res.push_back(point);
	}
	return res;
}