#include "scalarfield2D.h"
#include "mathUtils.h"
#include "texture2D.h"

/*!
\class Scalarfield2D scalarfield.h
\brief A 2 dimensional scalar field, implementing several useful functions
such as GetGLTexture(), Gradient(), Min/Max/Normalize()...
*/

/*
\brief Default Constructor
*/
ScalarField2D::ScalarField2D() : ValueField()
{
}

/*
\brief Constructor from an image specified by filePath
\param filePath image file path
\param blackAltitude min value
\param whiteAltitude max value
*/
ScalarField2D::ScalarField2D(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox) : ValueField(nx, ny, bbox)
{
	ReadFromImage(filePath, blackAltitude, whiteAltitude);
}

/*
\brief Constructor
\param nx size in x axis
\param ny size in z axis
\param bbox bounding box of the domain in world coordinates
*/
ScalarField2D::ScalarField2D(int nx, int ny, const Box2D& bbox) : ValueField(nx, ny, bbox)
{
}

/*
\brief Constructor
\param nx size in x axis
\param ny size in y axis
\param bbox bounding box of the domain
\param value default value of the field
*/
ScalarField2D::ScalarField2D(int nx, int ny, const Box2D& bbox, float value) : ValueField(nx, ny, bbox, value)
{
}

/*
\brief copy constructor
\param field Scalarfield2D to copy
*/
ScalarField2D::ScalarField2D(const ScalarField2D& field) : ValueField(field.nx, field.ny, field.box)
{
	for (unsigned int i = 0; i < values.size(); i++)
		values[i] = field.values[i];
}

/*
\brief Destructor
*/
ScalarField2D::~ScalarField2D() 
{
}

/*
\brief Compute the gradient for the vertex (i, j)
*/
Vector2 ScalarField2D::Gradient(int i, int j) const
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

/*
\brief
*/
void ScalarField2D::Add(int i, int j, float v)
{
	values[ToIndex1D(i, j)] += v;
}

/*
\brief
*/
void ScalarField2D::Remove(int i, int j, float v)
{
	values[ToIndex1D(i, j)] -= v;
}

/*
\brief Normalize this field 
*/
void ScalarField2D::NormalizeField()
{
	float min = Min();
	float max = Max();
	for (int i = 0; i < ny * nx; i++)
		values[i] = (values[i] - min) / (max - min);
}

/*
\brief Normalize this field between [min, max] values.
\param min min value
\param max max value
*/
void ScalarField2D::NormalizeField(float min, float max)
{
	for (int i = 0; i < ny * nx; i++)
		values[i] = (values[i] - min) / (max - min);
}

/*
\brief Return the normalized version of this field
*/
ScalarField2D ScalarField2D::Normalized() const
{
	ScalarField2D ret(*this);
	float min = Min();
	float max = Max();
	for (int i = 0; i < ny * nx; i++)
		ret.values[i] = (ret.values[i] - min) / (max - min);
	return ret;
}

/*
\brief Compute the average value of this field.
*/
float ScalarField2D::Average() const
{
	float ret = 0.0f;
	for (unsigned int i = 0; i < values.size(); i++)
		ret += values[i];
	return ret / float(values.size());
}

/*
\brief Get All points whose values are > threshold.
\param threshold minimum value of range
*/
std::vector<ScalarValue> ScalarField2D::FilterSuperiorTo(float threshold) const
{
	std::vector<ScalarValue> ret;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float v = values[ToIndex1D(i, j)];
			if (v > threshold)
				ret.push_back(ScalarValue(i, j, v));
		}
	}
	return ret;
}

/*
\brief Get All points whose values are < threshold.
\param threshold maximum value of range
*/
std::vector<ScalarValue> ScalarField2D::FilterInferiorTo(float threshold) const
{
	std::vector<ScalarValue> ret;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float v = values[ToIndex1D(i, j)];
			if (v < threshold)
				ret.push_back(ScalarValue(i, j, v));
		}
	}
	return ret;
}

/*
\brief Get All points whose values are in [min, max] range.
\param min minimum threshold
\param max maximum threshold
*/
std::vector<ScalarValue> ScalarField2D::FilterBetween(float min, float max) const
{
	std::vector<ScalarValue> ret;
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float v = values[ToIndex1D(i, j)];
			if (v >= min && v <= max)
				ret.push_back(ScalarValue(i, j, v));
		}
	}
	return ret;
}

/*
\brief Compute the cell size of X/Y in world coordinates.
*/
Vector2 ScalarField2D::CellSize() const
{
	Vector2 cellSize;
	cellSize.x = (box.Vertex(1)[0] - box.Vertex(0)[0]) / (nx - 1);
	cellSize.y = (box.Vertex(1)[1] - box.Vertex(0)[1]) / (ny - 1);
	return cellSize;
}

/*
\brief Compute a vertex world position including his height.
*/
Vector3 ScalarField2D::Vertex(int i, int j) const
{
	float x = box.Vertex(0).x + i * (box.Vertex(1).x - box.Vertex(0).x) / (nx - 1);
	float y = Get(i, j);
	float z = box.Vertex(0).y + j * (box.Vertex(1).y - box.Vertex(0).y) / (ny - 1);
	return Vector3(x, y, z);
}

/*
\brief Compute a vertex world position including his height.
*/
Vector3 ScalarField2D::Vertex(const Vector2i& v) const
{
	float x = box.Vertex(0).x + v.x * (box.Vertex(1).x - box.Vertex(0).x) / (nx - 1);
	float y = Get(v);
	float z = box.Vertex(0).y + v.y * (box.Vertex(1).y - box.Vertex(0).y) / (ny - 1);
	return Vector3(x, y, z);
}

/*
\brief Get Vertex world position by performing bilinear interpolation.
\param v world position in 2D
*/
Vector3 ScalarField2D::Vertex(const Vector2& v) const
{
	return Vector3(v.x, GetValueBilinear(v), v.y);
}

/*
\brief todo
*/
void ScalarField2D::ReadFromImage(const std::string& filePath, float blackValue, float whiteValue)
{
	Texture2D grayscaleTex = Texture2D(filePath);
	float texelX = 1.0f / (grayscaleTex.Width());
	float texelY = 1.0f / (grayscaleTex.GetValueBilinear());
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / float(nx - 1);
			float v = i / float(ny - 1);

			int anchorX = int((u * (grayscaleTex.Width() - 1)));
			int anchorY = int((v * (grayscaleTex.GetValueBilinear() - 1)));
			if (anchorX == grayscaleTex.Width() - 1)
				anchorX--;
			if (anchorY == grayscaleTex.GetValueBilinear() - 1)
				anchorY--;

			// Bilinear interpolation
			float a = grayscaleTex.Pixel(anchorX, anchorY).r;
			float b = grayscaleTex.Pixel(anchorX, anchorY + 1).r;
			float c = grayscaleTex.Pixel(anchorX + 1, anchorY + 1).r;
			float d = grayscaleTex.Pixel(anchorX + 1, anchorY).r;

			float anchorU = anchorX * texelX;
			float anchorV = anchorY * texelY;

			float localU = (u - anchorU) / texelX;
			float localV = (v - anchorV) / texelY;

			float abu = Math::Lerp(a, b, localV);
			float dcu = Math::Lerp(d, c, localV);

			float value = Math::Lerp(abu, dcu, localU);
			Set(i, j, blackValue + value * (whiteValue - blackValue));
		}
	}
}

/*
\brief Utility method to save the scalarfield as image.
\param path relative path
*/
void ScalarField2D::SaveAsImage(const std::string& path)
{
	std::cout << "SaveAsImage : " << path << std::endl;
}

/*
\brief Utility method to render a scalarfield as a GL texture.
\param unit gl texture unit.
*/
GLuint ScalarField2D::GetGLTexture(int unit) const
{
	Texture2D tex = Texture2D(nx, ny);
	float min = Min();
	float max = Max();
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float v = (Get(i, j) - min) / (max - min);
			tex.SetPixel(i, j, Color(v, v, v, 1.0));
		}
	}
	return tex.GetGLTexture(unit, true);
}
