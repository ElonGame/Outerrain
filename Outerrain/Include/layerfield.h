#pragma once

#include "meshSet.h"
#include "heightfield.h"
#include "ecosystem.h"

class LayerField : public Heightfield
{
protected:
	Scalarfield2D sediments;
	Scalarfield2D rocks;
	Scalarfield2D water;
	Scalarfield2D snow;
	Scalarfield2D vegetation;

public:
	LayerField(int nx, int ny, const Box2D& bbox);
	LayerField(int nx, int ny, const Box2D& bbox, float value);
	LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox);

	Vector3 Vertex(int i, int j) const;
	float Height(const Vector2& p) const;

	void LightingEventSimulate(float strength, int probability, int fireProbability);
	void LightingEvent(const Vector2& position, float strength, int fireProbability);
	void FireEventSimulate(const Vector2& startPosition);

	void Stabilize();

	MeshSetRenderer GetVegetationInstances() const;
	MeshSetRenderer GetRockInstances() const;
};
