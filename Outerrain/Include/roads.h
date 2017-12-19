#pragma once
#include <vector>
#include "vec.h"
#include "terrain.h"

class ShortestPath
{
public:
	static std::vector<Vector3> FindShortestPath(const VegetationTerrain&, const Vector3&, const Vector3&);
	static float GetPositionScore(const Vector3&, const Vector3&, const ScalarField2D&, const VegetationTerrain&);

	static GameObject* GetNodeObject();
};