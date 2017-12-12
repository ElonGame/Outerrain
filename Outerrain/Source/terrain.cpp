#include <algorithm>
#include <math.h>
#include <deque>
#include <queue>
#include "terrain.h"
#include "vec.h"
#include "perlin.h"
#include "image.h"
#include "vegetationObject.h"
#include "gameobject.h"

#define max(a, b) a > b ? a : b

static bool compareHeight(Vector3 u, Vector3 v)
{
	return (u.y > v.y);
}

/* Terrain2D */
Terrain2D::Terrain2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	heightField = ScalarField2D(nx, ny, bottomLeft, topRight);
	normalField = ValueField<Vector3>(nx, ny, bottomLeft, topRight);
}

void Terrain2D::InitFromFile(const char* file, float blackAltitude, float whiteAltitude)
{
	Image heightmap;
	heightmap.ReadImage(file);
	float texelX = 1.0f / (heightmap.Width());
	float texelY = 1.0f / (heightmap.Height());

	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / ((float)nx - 1);
			float v = i / ((float)ny - 1);

			int anchorX = u * (heightmap.Width() - 1);
			int anchorY = v * (heightmap.Height() - 1);
			if (anchorX == heightmap.Width() - 1)
				anchorX--;
			if (anchorY == heightmap.Height() - 1)
				anchorY--;

			float a = heightmap(anchorX, anchorY).r;
			float b = heightmap(anchorX, anchorY + 1).r;
			float c = heightmap(anchorX + 1, anchorY + 1).r;
			float d = heightmap(anchorX + 1, anchorY).r;

			float anchorU = anchorX * texelX;
			float anchorV = anchorY * texelY;

			float localU = (u - anchorU) / texelX;
			float localV = (v - anchorV) / texelY;

			float abu = Lerp(a, b, localU);
			float dcu = Lerp(d, c, localU);

			float value = Lerp(dcu, abu, localV);
			heightField.Set(i, j, blackAltitude + value * (whiteAltitude - blackAltitude));
		}
	}
	ComputeNormalField();
}

void Terrain2D::InitFromNoise(int blackAltitude, int whiteAltitude)
{
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			//float v = blackAltitude + Noise::ValueNoise2D(i, j, 14589) * (whiteAltitude - blackAltitude);
			float v = blackAltitude + Perlin::Perlin2DAt(1.0, 2.0, 0.5, 6.0, i, j) * (whiteAltitude - blackAltitude);
			heightField.Set(i, j, v);
		}
	}
	ComputeNormalField();
}

double Terrain2D::Height(const Vector2& p) const
{
	return heightField.GetValueBilinear(p);
}

Vector3 Terrain2D::Normal(int i, int j) const
{
	return normalField.Get(i, j);
}

Vector3 Terrain2D::Vertex(int i, int j) const
{
	double x = bottomLeft.x + j * (topRight.x - bottomLeft.x) / (nx - 1);
	double y = heightField.Get(i, j);
	double z = bottomLeft.y + i * (topRight.y - bottomLeft.y) / (ny - 1);
	return Vector3(x, y, z);
}

Mesh* Terrain2D::GetMesh() const
{
	Mesh* ret = new Mesh();
	ret->CalculateFromTerrain2D(*this);
	return ret;
}

void Terrain2D::SetHeight(int i, int j, double v)
{
	heightField.Set(i, j, v);
}

float Terrain2D::Lerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
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

double Terrain2D::NormalizedHeight(const Vector2& p) const
{
	float h = Height(p);
	return h / heightField.MaxValue();
}

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

void Terrain2D::Erode(int index, double epsilon)
{
	heightField.Set(index, heightField.Get(index) - epsilon);
}

void Terrain2D::Transport(int index, double epsilon)
{
	heightField.Set(index, heightField.Get(index) + epsilon);
}

// @TODO Diviser currentSlope et currentHeight par c.x en horizontal et c.y en vertical et norme en diagonale
int Terrain2D::Distribute(Point p, Point* neighbours, float* height, float* slope) const
{
	int i = p.x, j = p.y;

	int counter = 0;
	double currentSlope = 0.0f;
	double currentHeight = 0.0f;
	double pointHeight = heightField.Get(i, j);

	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{
			if (k == 0 || l == 0 || heightField.InsideVertex(i + k, j + l) == false)
				continue;

			double neighHeight = heightField.Get(i + k, j + l);
			if (pointHeight > neighHeight)
			{
				currentHeight = pointHeight - neighHeight;
				if (k + l == -1 || k + l == 1)
					currentSlope = currentHeight;
				else
					currentSlope = currentHeight / sqrt(2);

				neighbours[counter] = Point(i + k, j + l);
				height[counter] = currentHeight;
				slope[counter] = currentSlope;
				counter++;
			}
		}
	}
	return counter;
}

ScalarField2D Terrain2D::Drainage() const
{
	std::deque<Vector3> points;
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			points.push_back(Vector3(i, heightField.Get(i, j), j));
		}
	}
	std::sort(points.begin(), points.end(), compareHeight);

	ScalarField2D drainage = ScalarField2D(nx, ny, bottomLeft, topRight, 1.0);
	while (!points.empty())
	{
		Vector3 p = points.front();
		points.pop_front();

		int i = p.x, j = p.z;
		Point neighbours[8];
		float slope[8];
		float height[8];
		int n = Distribute(Point(i, j), neighbours, height, slope);

		double sum = 0.0;
		for (int k = 0; k < n; k++)
			sum += slope[k];

		for (int k = 0; k < n; k++)
		{
			int l = neighbours[k].x, m = neighbours[k].y;
			drainage.Set(l, m, drainage.Get(l, m) + drainage.Get(i, j) * (slope[k] / sum));
		}
	}
	drainage.WriteImageGrayscale("Data/drainage.png");
	return drainage;
}

ScalarField2D Terrain2D::DrainageSqrt() const
{
	ScalarField2D drainageField = Drainage();
	ScalarField2D sqrtDrainageField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			sqrtDrainageField.Set(i, j, sqrt(drainageField.Get(i, j)));
		}
	}
	sqrtDrainageField.WriteImageGrayscale("Data/drainagesqrt.png");
	return sqrtDrainageField;
}

ScalarField2D Terrain2D::WetnessField() const
{
	ScalarField2D drainageField = Drainage();
	ScalarField2D slopeField = SlopeField();
	ScalarField2D wetnessField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			wetnessField.Set(i, j, log(drainageField.Get(i, j) / (1 + slopeField.Get(i, j))));
		}
	}
	wetnessField.WriteImageGrayscale("Data/wetness.png");
	return wetnessField;
}

ScalarField2D Terrain2D::StreamPowerField() const
{
	ScalarField2D drainageField = Drainage();
	ScalarField2D slopeField = SlopeField();
	ScalarField2D streamPowerField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			streamPowerField.Set(i, j, sqrt(drainageField.Get(i, j)) * slopeField.Get(i, j));
		}
	}
	streamPowerField.WriteImageGrayscale("Data/streampower.png");
	return streamPowerField;
}

double Terrain2D::ComputeIllumination(int i, int j) const
{
	ScalarField2D slopeField = SlopeField();

	double epsilon = 10 ^ -2;
	Vector3 p = Vertex(i, j) + Vector3(0.0, epsilon, 0.0);

	return 0.0;
	/*

	Pour tout k
		On compte le nombre dintersectons entre terrain et rayon. Intersect(Ray(p, direction aléatoire));

	SF2 slopeField = slope()
	k = max(slopeField)
	DeltaY = Y(qi) - Yterrain(qi) > 0 < 0
	step = DeltaY / K;
	*/

	// Classe Ray
	// Fonction Inside(q) : On calcul le Y du terrain, et on regarde si on est au dessus ou en dessous. Comparaison Yterrain vs Yq
}

ScalarField2D Terrain2D::AccessibilityField() const
{
	ScalarField2D accessibilityField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			accessibilityField.Set(i, j, ComputeIllumination(i, j));
		}
	}
	return accessibilityField;
}


/* LayerTerrain2D */
LayerTerrain2D::LayerTerrain2D(int nx, int ny, Vector2 a, Vector2 b)
	: nx(nx), ny(ny), a(a), b(b)
{
	sand = ScalarField2D(nx, ny, a, b);
	bedrock = ScalarField2D(nx, ny, a, b);
}

double LayerTerrain2D::Height(int i, int j) const
{
	return BeckrockValue(i, j) + SandValue(i, j);
}

double LayerTerrain2D::BeckrockValue(int i, int j) const
{
	return bedrock.Get(i, j);
}

double LayerTerrain2D::SandValue(int i, int j) const
{
	return sand.Get(i, j);
}

void LayerTerrain2D::ThermalErosion(int stepCount)
{
	for (int k = 0; k < stepCount; k++)
	{
		std::queue<int> instables;
		std::queue<int> matter; // quantit� de mati�re qui est transport�e
		for (int i = 0; i < bedrock.SizeX(); i++)
		{
			for (int j = 0; j < bedrock.SizeY(); j++)
			{
				// On calcule le delta H 
				// Pour �a on fait le max des delta H des voisins de I
				//double dH = bedrock.At(i,j) - bedrock.At(i, j + 1);
				double dH1 = 0;
				double dH2 = 0;
				double dH3 = 0;
				double dH4 = 0;
				if (j < bedrock.SizeY() - 1) {
					dH1 = bedrock.Get(i, j) - bedrock.Get(i, j + 1);
				}
				if (i < bedrock.SizeX() - 1)
					dH2 = bedrock.Get(i, j) - bedrock.Get(i + 1, j);
				if (i > 0)
					dH3 = bedrock.Get(i, j) - bedrock.Get(i - 1, j);
				if (j > 0)
					dH4 = bedrock.Get(i, j) - bedrock.Get(i, j - 1);
				double dH = max(dH1, max(dH2, max(dH3, dH4)));

				//Pour calculer stress, c'est � dire quantit� de mati�re � �roder :
				// stress = k * dH
				double k = 0.1;
				//alpha = 40� +- 5�
				double alpha = 42;
				double delta = abs(bedrock.TopRight().x - bedrock.BottomLeft().x) / bedrock.SizeX();
				if (abs(dH) / delta > tan(alpha))
				{
					//AddToQueue INSTABLE
					instables.push(bedrock.Index(i, j));
					matter.push(dH*k);
				}
			}
		}
		while (!instables.empty()) {
			int index = instables.front();
			instables.pop();
			int indexLVoisin = bedrock.LowestNeighbor(index);
			double eps = matter.front();
			matter.pop();
			bedrock.Set(index, bedrock.Get(index) - eps);
			sand.Set(indexLVoisin, eps);
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


/* VegetationTerrain */
VegetationTerrain::VegetationTerrain(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: Terrain2D(nx, ny, bottomLeft, topRight),
	vegetationDensityField(nx, ny, bottomLeft, topRight)
{
}

void VegetationTerrain::ComputeDensities()
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

	int tileCountX = (topRight.x - bottomLeft.x) / tileSize + 1;
	int tileCountY = (topRight.y - bottomLeft.y) / tileSize + 1;

	std::vector<GameObject*> vegObjects;
	for (int i = 0; i < tileCountY; i++)
	{
		for (int j = 0; j < tileCountX; j++)
		{
			for (int x = 0; x < points.size(); x++)
			{
				Vector2 point = bottomLeft
					+ Vector2(tileSize, 0) * (float)j
					+ Vector2(0, tileSize) * (float)i
					+ points[x];
				if (vegetationDensityField.IsInsideField(point) == true)
				{
					float density = vegetationDensityField.GetValueBilinear(point);
					float p = rand() / (float)RAND_MAX;
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
		float randX = rand() / (float)RAND_MAX;
		float randY = rand() / (float)RAND_MAX;
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