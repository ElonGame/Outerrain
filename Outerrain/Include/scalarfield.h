#pragma once

#include "GL\glew.h"
#include "valueField.h"

class Scalarfield2D : public ValueField<float>
{
public:
	Scalarfield2D();
	Scalarfield2D(int nx, int ny, const Vector2& bottomLeft, const Vector2& topRight);
	Scalarfield2D(int nx, int ny, const Vector2& bottomLeft, const Vector2& topRight, float value);
	~Scalarfield2D() { }

	Vector3 Vertex(int i, int j) const;
	Vector3 Vertex(const Vector2i& v) const;

	Vector2 CellSize() const;
	Vector2 Gradient(int i, int j) const;
	void NormalizeField();
	void NormalizeField(float min, float max);
	float Average() const;

	void SaveAsImage(const char* path);
	void ReadFromImage(const char* file, int blackAltitude, int whiteAltitude);

	GLuint GetGLTexture() const;
};
