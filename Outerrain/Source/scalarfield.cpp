#include "scalarfield.h"
#include "mathUtils.h"
#include "image.h"
#include "texture.h"

Scalarfield2D::Scalarfield2D() : ValueField()
{
}

Scalarfield2D::Scalarfield2D(int nx, int ny, const Box2D& bbox) : ValueField(nx, ny, bbox)
{
}

Scalarfield2D::Scalarfield2D(int nx, int ny, const Box2D& bbox, float value) : ValueField(nx, ny, bbox, value)
{
}

Scalarfield2D::Scalarfield2D(const Scalarfield2D& field) : ValueField(field.nx, field.ny, field.box)
{
	for (unsigned int i = 0; i < values.size(); i++)
		values[i] = field.values[i];
}

Vector2 Scalarfield2D::Gradient(int i, int j) const
{
	Vector2 ret;
	float cellSizeX = (box.Vertex(1).x - box.Vertex(0).x) / (nx - 1);
	float cellSizeY = (box.Vertex(1).y - box.Vertex(0).y) / (ny - 1);

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
	float min = Min();
	float max = Max();
	for (int i = 0; i < ny * nx; i++)
		values[i] = (values[i] - min) / (max - min);
}

void Scalarfield2D::NormalizeField(float min, float max)
{
	for (int i = 0; i < ny * nx; i++)
		values[i] = (values[i] - min) / (max - min);
}

Scalarfield2D Scalarfield2D::Normalized() const
{
	Scalarfield2D ret(*this);
	float min = Min();
	float max = Max();
	for (int i = 0; i < ny * nx; i++)
		ret.values[i] = (ret.values[i] - min) / (max - min);
	return ret;
}

float Scalarfield2D::Average() const
{
	float ret = 0.0f;
	for (unsigned int i = 0; i < values.size(); i++)
		ret += values[i];
	return ret / static_cast<float>(values.size());
}

Vector2 Scalarfield2D::CellSize() const
{
	Vector2 cellSize;
	cellSize.x = (box.Vertex(1)[0] - box.Vertex(0)[0]) / (nx - 1);
	cellSize.y = (box.Vertex(1)[1] - box.Vertex(0)[1]) / (ny - 1);
	return cellSize;
}

Vector3 Scalarfield2D::Vertex(int i, int j) const
{
	float x = box.Vertex(0).x + j * (box.Vertex(1).x - box.Vertex(0).x) / (nx - 1);
	float y = Get(i, j);
	float z = box.Vertex(0).y + i * (box.Vertex(1).y - box.Vertex(0).y) / (ny - 1);
	return Vector3(x, y, z);
}

Vector3 Scalarfield2D::Vertex(const Vector2i& v) const
{
	float x = box.Vertex(0).x + v.y * (box.Vertex(1).x - box.Vertex(0).x) / (nx - 1);
	float y = Get(v);
	float z = box.Vertex(0).y + v.x * (box.Vertex(1).y - box.Vertex(0).y) / (ny - 1);
	return Vector3(x, y, z);
}

void Scalarfield2D::SaveAsImage(const char* path)
{
	Image im = Image(nx, ny);
	float min = Min();
	float max = Max();
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

void Scalarfield2D::ReadFromImage(const char* file, int blackAltitude, int whiteAltitude)
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

			float abu = Math::Lerp(a, b, localV);
			float dcu = Math::Lerp(d, c, localV);

			float value = Math::Lerp(abu, dcu, localU);
			Set(i, j, blackAltitude + value * (whiteAltitude - blackAltitude));
		}
	}
}

GLuint Scalarfield2D::GetGLTexture(int unit) const
{
	Image im = Image(nx, ny);
	float min = Min();
	float max = Max();
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float v = (Get(i, j) - min) / (max - min);
			im(j, i) = Color(v, v, v, 1.0);
		}
	}
	return MakeTexture(unit, im, GL_RGBA);
}
