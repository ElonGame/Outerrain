#include "roads.h"
#include "gameobject.h"
using namespace std;


const GraphNode ShortestPath::GetNeighbourInGraph(const std::vector<std::vector<GraphNode>>& g, int i, int j, int k)
{
	if (k == 0)
		return g[i - 1][j];
	if (k == 1)
		return g[i + 1][j];
	if (k == 2)
		return g[i][j - 1];
	if (k == 3)
		return g[i][j + 1];
	if (k == 4)
		return g[i - 1][j - 1];
	if (k == 5)
		return g[i - 1][j + 1];
	if (k == 6)
		return g[i + 1][j - 1];
	if (k == 7)
		return g[i + 1][j + 1];
}

void ShortestPath::ComputeGraph(const Terrain2D& terrain)
{
	// Compute graph from input terrain
	vector<vector<GraphNode>> nodes;
	nodes.resize(terrain.SizeY());
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].resize(terrain.SizeX());
		for (int j = 0; j < nodes[i].size(); j++)
		{
			if (i > 0)
			{
				nodes[i][j].W[0] = 1.0f;
				nodes[i - 1][j].W[1] = 1.0f;
			}
			if (i < nodes.size() - 1)
			{
				nodes[i][j].W[1] = 1.0f;
				nodes[i + 1][j].W[0] = 1.0f;
			}
			if (j > 0)
			{
				nodes[i][j].W[2] = 1.0f;
				nodes[i][j - 1].W[3] = 1.0f;
			}
			if (j < nodes[i].size() - 1) 
			{
				nodes[i][j].W[3] = 1.0f;
				nodes[i][j + 1].W[2] = 1.0f;
			}
			if (i > 0 && j > 0)
			{
				nodes[i][j].W[4] = 1.0f;
				nodes[i - 1][j - 1].W[7] = 1.0f;
			}
			if (i > 0 && j < nodes[i].size())
			{
				nodes[i][j].W[5] = 1.0f;
				nodes[i - 1][j + 1].W[6] = 1.0f;
			}
			if (i < nodes.size() && j > 0)
			{
				nodes[i][j].W[6] = 1.0f;
				nodes[i + 1][j - 1].W[5] = 1.0f;
			}
			if (i < nodes.size() && j < nodes[i].size())
			{
				nodes[i][j].W[7] = 1.0f;
				nodes[i + 1][j + 1].W[4] = 1.0f;
			}
		}
	}

	// Compute edge weight for each node
	ScalarField2D slopefield = terrain.SlopeField();
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i].size(); j++)
		{
			for (int k = 0; k < ADJ_SIZE; k++)
			{
				if (nodes[i][j].W[k] != NULL_VALUE)
				{
					GraphNode node = nodes[i][j];
					GraphNode neighbour = GetNeighbourInGraph(nodes, i, j, k);
					// Todo : compute edge weight between node and neighbour
					// With slope.
				}
			}
		}
	}
}
