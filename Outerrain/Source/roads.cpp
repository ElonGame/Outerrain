#include "roads.h"
#include "gameobject.h"
using namespace std;

const GraphNode ShortestPath::GetNeighbourInGraph(const std::vector<std::vector<GraphNode>>& g, int i, int j, int& i2, int& j2, int& k2, int k)
{
	if (k == 0)
	{
		i2 = i + 1;
		j2 = j;
		k2 = 1;
		return g[i - 1][j];
	}
	if (k == 1)
	{
		i2 = i - 1;
		j2 = j;
		k2 = 0;
		return g[i + 1][j];
	}
	if (k == 2)
	{
		i2 = i;
		j2 = j + 1;
		k2 = 3;
		return g[i][j - 1];
	}
	if (k == 3)
	{
		i2 = i;
		j2 = j - 1;
		k2 = 2;
		return g[i][j + 1];
	}
	if (k == 4)
	{
		i2 = i + 1;
		j2 = j + 1;
		k2 = 7;
		return g[i - 1][j - 1];
	}
	if (k == 5)
	{
		i2 = i + 1;
		j2 = j - 1;
		k2 = 6;
		return g[i - 1][j + 1];
	}
	if (k == 6)
	{
		i2 = i - 1;
		j2 = j + 1;
		k2 = 5;
		return g[i + 1][j - 1];
	}
	if (k == 7)
	{
		i2 = i - 1;
		j2 = j - 1;
		k2 = 4;
		return g[i + 1][j + 1];
	}
}

void ShortestPath::ComputeNeighbourForNode(std::vector<std::vector<GraphNode>>& nodes, int i, int j)
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

void ShortestPath::ComputeGraph(const Terrain2D& terrain)
{
	// Compute graph from input terrain
	vector<vector<GraphNode>> nodes;
	nodes.resize(terrain.SizeY());
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].resize(terrain.SizeX());
		for (int j = 0; j < nodes[i].size(); j++)
			ComputeNeighbourForNode(nodes, i, j);
	}

	// Compute edge weight for each node
	ScalarField2D slopefield = terrain.SlopeField();
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i].size(); j++)
		{
			for (int k = 0; k < ADJ_SIZE; k++)
			{
				if (nodes[i][j].W[k] == NULL_VALUE)
					continue;

				int i2, j2, k2;
				GraphNode node = nodes[i][j];
				GraphNode neighbour = GetNeighbourInGraph(nodes, i, j, i2, j2, k2, k);
				Vector3 v1 = terrain.Vertex(i, j);
				Vector3 v2 = terrain.Vertex(i2, j2);
				float w = Magnitude(v2 - v1) * (1.0f + (v2.y - v1.y));
				nodes[i][j].W[k] = w;
				nodes[i2][j2].W[k2] = w;
			}
		}
	}
}
