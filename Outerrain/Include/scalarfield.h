#pragma once

#include "GL/glew.h"
#include "valueField.h"

class Scalarfield2D : public ValueField<float>
{
public:
	Scalarfield2D();
	Scalarfield2D(const Scalarfield2D& field);
	Scalarfield2D(int nx, int ny, const Box2D& bbox);
	Scalarfield2D(int nx, int ny, const Box2D& bbox, float value);
	~Scalarfield2D() { }

	Vector3 Vertex(int i, int j) const;
	Vector3 Vertex(const Vector2i& v) const;

	Vector2 CellSize() const;
	Vector2 Gradient(int i, int j) const;
	Scalarfield2D Normalized() const;
	void NormalizeField();
	void NormalizeField(float min, float max);
	float Average() const;

	void SaveAsImage(const std::string& filePath);
	void ReadFromImage(const std::string filePath, float blackAltitude, float whiteAltitude);

	GLuint GetGLTexture(int unit) const;
};
