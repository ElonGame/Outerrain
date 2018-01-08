#pragma once
#include <vector>
#include "vec.h"
#include "terrain.h"
#include "graphs.h"

class ShortestPath
{
public:
	static const GraphNode GetNeighbourInGraph(const std::vector<std::vector<GraphNode>>& g, int i, int j, int k);
	static void ComputeNeighbourForNode(std::vector<std::vector<GraphNode>>& g, int i, int j);
	static void ComputeGraph(const Terrain2D& terrain);
};