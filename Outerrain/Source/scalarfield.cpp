#include "scalarfield.h"


Scalarfield2D::Scalarfield2D() : ValueField()
{
}

Scalarfield2D::Scalarfield2D(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight)
	: ValueField(nx, ny, bottomLeft, topRight)
{
}

Scalarfield2D::Scalarfield2D(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight, const float& value)
	: ValueField(nx, ny, bottomLeft, topRight, value)
{
}

Vector2 Scalarfield2D::Gradient(const int& i, const int& j) const
{
	Vector2 ret;
	float cellSizeX = (topRight.x - bottomLeft.x) / (nx - 1);
	float cellSizeY = (topRight.y - bottomLeft.y) / (ny - 1);

	// X Gradient
	if (i == 0)
		ret.x = (Get(i + 1, j) - Get(i, j)) / cellSizeX;
	else if (i == ny - 1)
		ret.x = (Get(i, j) - Get(i - 1, j)) / cellSizeX;
	else
		ret.x = (Get(i + 1, j) - Get(i - 1, j)) / (2.0f * cellSizeX);

	// Y Gradient
	if (j == 0)
		ret.y = (Get(i, j + 1) - Get(i, j)) / cellSizeY;
	else if (j == nx - 1)
		ret.y = (Get(i, j) - Get(i, j - 1)) / cellSizeY;
	else
		ret.y = (Get(i, j + 1) - Get(i, j - 1)) / (2.0f * cellSizeY);

	return ret;
}

void Scalarfield2D::NormalizeField()
{
	float min = MinValue();
	float max = MaxValue();
	for (int i = 0; i < ny * nx; i++)
		values[i] = (values[i] - min) / (max - min);
}

void Scalarfield2D::NormalizeField(const float& min, const float& max)
{
	for (int i = 0; i < ny * nx; i++)
		values[i] = (values[i] - min) / (max - min);
}

float Scalarfield2D::AverageValue() const
{
	float ret = 0.0f;
	for (unsigned int i = 0; i < values.size(); i++)
		ret += values[i];
	return ret / static_cast<float>(values.size());
}

Vector3 Scalarfield2D::Vertex(const int& i, const int& j) const
{
	float x = bottomLeft.x + j * (topRight.x - bottomLeft.x) / (nx - 1);
	float y = Get(i, j);
	float z = bottomLeft.y + i * (topRight.y - bottomLeft.y) / (ny - 1);
	return Vector3(x, y, z);
}

Vector3 Scalarfield2D::Vertex(const Vector2i& v) const
{
	float x = bottomLeft.x + v.y * (topRight.x - bottomLeft.x) / (nx - 1);
	float y = Get(v);
	float z = bottomLeft.y + v.x * (topRight.y - bottomLeft.y) / (ny - 1);
	return Vector3(x, y, z);
}

void Scalarfield2D::WriteImageGrayscale(const char* path)
{
	Image im = Image(nx, ny);
	float min = MinValue();
	float max = MaxValue();
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float v = (Get(i, j) - min) / (max - min);
			im(j, i) = Color(v, v, v, 1.0);
		}
	}
	im.WriteImage(path, true);
}

void Scalarfield2D::ReadImageGrayscale(const char* file, int blackAltitude, int whiteAltitude)
{
	Image heightmap;
	heightmap.ReadImage(file, false);
	float texelX = 1.0f / (heightmap.Width());
	float texelY = 1.0f / (heightmap.Height());

	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / (static_cast<float>(nx - 1));
			float v = i / (static_cast<float>(ny - 1));

			int anchorX = static_cast<int>((u * (heightmap.Width() - 1)));
			int anchorY = static_cast<int>((v * (heightmap.Height() - 1)));
			if (anchorX == heightmap.Width() - 1)
				anchorX--;
			if (anchorY == heightmap.Height() - 1)
				anchorY--;

			// Bilinear interpolation
			float a = heightmap(anchorX, anchorY).r;
			float b = heightmap(anchorX, anchorY + 1).r;
			float c = heightmap(anchorX + 1, anchorY + 1).r;
			float d = heightmap(anchorX + 1, anchorY).r;

			float anchorU = anchorX * texelX;
			float anchorV = anchorY * texelY;

			float localU = (u - anchorU) / texelX;
			float localV = (v - anchorV) / texelY;

			float abu = Lerp(a, b, localV);
			float dcu = Lerp(d, c, localV);

			float value = Lerp(abu, dcu, localU);
			Set(i, j, blackAltitude + value * (whiteAltitude - blackAltitude));
		}
	}
}
