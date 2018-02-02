#include "heightfield.h"
#include "vec.h"
#include <numeric>


Heightfield::Heightfield()
	: Scalarfield2D()
{
}

Heightfield::Heightfield(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight)
	: Scalarfield2D(nx, ny, bottomLeft, topRight)
{
}

Heightfield::Heightfield(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight, const float& value)
	: Scalarfield2D(nx, ny, bottomLeft, topRight, value)
{

}

Heightfield::Heightfield(const std::string& filePath, const int& minAltitude, const int& maxAltitude, const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight)
	: Heightfield(nx, ny, bottomLeft, topRight)
{
	ReadImageGrayscale(filePath.c_str(), minAltitude, maxAltitude);
}


void Heightfield::Thermal(const int& stepCount, const float& E)
{
	// @Todo
}

void Heightfield::StreamPowerErosion(const int& stepCount, const float& E)
{
	// @Todo
}


Scalarfield2D Heightfield::DrainageArea() const
{
	Scalarfield2D DA = Scalarfield2D(nx, ny, bottomLeft, topRight, 1.0);
	std::array<float, 8> slopes;
	std::array<Vector2i, 8> coords;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			// Compute neighbours information
			slopes.fill(0.0f);
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
							slopes[neighbourCount] = dH / sqrt(2.0);

						coords[neighbourCount] = Vector2i(i + k, j + l);
						neighbourCount++;
					}
				}
			}
			// Distribute to those lower neighbours
			float sum = Accumulate<float, 8>(slopes);
			for (int k = 0; k < neighbourCount; k++)
				DA.Set(coords[k], DA.Get(coords[k]) + DA.Get(i, j) * (slopes[k] / sum));
		}
	}
	return DA;
}

Scalarfield2D Heightfield::Slope() const
{
	Scalarfield2D S = Scalarfield2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			S.Set(i, j, Magnitude(Gradient(i, j)));
	}
	return S;
}

Scalarfield2D Heightfield::Wetness() const
{
	Scalarfield2D DA = DrainageArea();
	Scalarfield2D S = Slope();
	Scalarfield2D W = Scalarfield2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			W.Set(i, j, abs(log(DA.Get(i, j) / (1.0f + S.Get(i, j)))));
	}
	return W;
}

Scalarfield2D Heightfield::StreamPower() const
{
	Scalarfield2D DA = DrainageArea();
	Scalarfield2D S = Slope();
	Scalarfield2D SP = Scalarfield2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			SP.Set(i, j, sqrt(DA.Get(i, j)) * S.Get(i, j));
	}
	return SP;
}

Scalarfield2D Heightfield::Illumination() const
{
	const int rayCount = 32;
	const float epsilon = 0.01f;
	const float K = Slope().Max();
	Scalarfield2D I = Scalarfield2D(nx, ny, bottomLeft, topRight);
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
				float angleH = (rand() % 360) * 0.0174533f;
				float angleV = rand() / static_cast<float>(RAND_MAX);
				Vector3 rayDir = Vector3(cos(angleH), 0.0f, sin(angleH));
				rayDir = Slerp(rayDir, Vector3(0.0f, 1.0f, 0.0f), angleV);
				if (Intersect(Ray(rayPos, rayDir), rayHit, K) == true)
					intersectionCount++;
			}
			I.Set(i, j, 1.0f - (intersectionCount / static_cast<float>(rayCount)));
		}
	}
	return I;
}


bool Heightfield::Intersect(const Ray& ray, Hit& hit, const float& K) const
{
	float step = (ray.origin.y - GetValueBilinear(Vector2(ray.origin.x, ray.origin.z))) / K;
	while (true)
	{
		Vector3 q = ray.origin + ray.direction * step;
		Vector2 rayPos2D = Vector2(q.x, q.z);
		if (Inside(rayPos2D) == false)
			break;
		float delta = q.y - GetValueBilinear(rayPos2D);
		if (delta <= 0.01f)
		{
			hit.position = q;
			hit.normal = Vector3(0);
			return true;
		}
		step += delta / K;
	}
	return false;
}

bool Heightfield::Intersect(const Ray& ray, Hit& hit) const
{
	return Intersect(ray, hit, 100.0f);
}

bool Heightfield::Intersect(const Vector3& origin, const Vector3 direction, Vector3& hitPos, Vector3& hitNormal) const
{
	Hit hit;
	bool res = Intersect(Ray(origin, direction), hit, 100.0f);
	hitPos = hit.position;
	hitNormal = hit.normal;
	return res;
}


Mesh* Heightfield::CreateMesh() const
{
	Mesh* ret = new Mesh();
	ValueField<Vector3> normals = ValueField<Vector3>(nx, ny, bottomLeft, topRight, Vector3(0));
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