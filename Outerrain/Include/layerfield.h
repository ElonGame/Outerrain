#pragma once

#include "scalarfield.h"
#include "meshModel.h"
#include "ecosystem.h"

class LayerField
{
protected:
	Scalarfield2D bedrock;
	Scalarfield2D sediments;
	Scalarfield2D rocks;
	Scalarfield2D water;
	Scalarfield2D snow;
	Scalarfield2D vegetation;

public:
	LayerField(int nx, int ny, const Box2D& bbox);
	LayerField(int nx, int ny, const Box2D& bbox, float value);
	LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox);

	int ToIndex1D(int i, int j) const;
	Vector3 Vertex(int i, int j) const;
	float Height(const Vector2& p) const;
	int SizeX() const;
	int SizeY() const;
	Box2D GetBox() const;

	void LightingEventSimulate(float strength, int probability, int fireProbability);
	void LightingEvent(const Vector2& position, float strength, int fireProbability);
	void FireEventSimulate(const Vector2& startPosition);

	void Stabilize();

	std::vector<EcosystemInstance> GetVegetationInstances() const;
	std::vector<EcosystemInstance> GetRockInstances() const;
	MeshModel* GetHeightfieldMesh() const;
};
