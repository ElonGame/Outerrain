#include "graph.h"

static const weight_t max_weight = std::numeric_limits<double>::infinity();

void ShortestPath::DijkstraComputePaths(vertex_t source, const adjacency_list_t &adjacency_list, std::vector<weight_t> &min_distance, std::vector<vertex_t> &previous)
{
	int n = adjacency_list.size();
	min_distance.clear();
	min_distance.resize(n, max_weight);
	min_distance[source] = 0;
	previous.clear();
	previous.resize(n, -1);
	std::set<std::pair<weight_t, vertex_t> > vertex_queue;
	vertex_queue.insert(std::make_pair(min_distance[source], source));

	while (!vertex_queue.empty())
	{
		weight_t dist = vertex_queue.begin()->first;
		vertex_t u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());

		// Visit each edge exiting u
		const std::vector<neighbor> &neighbors = adjacency_list[u];
		for (auto neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); neighbor_iter++)
		{
			vertex_t v = neighbor_iter->target;
			weight_t weight = neighbor_iter->weight;
			weight_t distance_through_u = dist + weight;
			if (distance_through_u < min_distance[v])
			{
				vertex_queue.erase(std::make_pair(min_distance[v], v));
				min_distance[v] = distance_through_u;
				previous[v] = u;
				vertex_queue.insert(std::make_pair(min_distance[v], v));
			}
		}
	}
}

std::list<vertex_t> ShortestPath::DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{
	std::list<vertex_t> path;
	while (vertex != -1)
	{
		path.push_front(vertex);
		vertex = previous[vertex];
		if (vertex >= previous.size() && vertex != -1)
		{
			std::cout << "Error! : " << vertex << std::endl;
			break;
		}
	}
	return path;
}

static void AddNeighbour(adjacency_list_t& graph, const VegetationTerrain& terrain, const Vector3& v1, int index1D, int i, int j)
{
	Vector3 v = terrain.Vertex(i, j);
	float w = Magnitude(v - v1) * (1.0f + (v.y - v1.y));
	int index1DN = i * terrain.SizeY() + j;
	w = abs(w);
	graph[index1D].push_back(neighbor(index1DN, w));
}

std::list<vertex_t> ShortestPath::FindPath(const VegetationTerrain& terrain, int iStart, int jStart, int iEnd, int jEnd, NeighborhoodType neighbourType)
{
	int nodeCount = terrain.SizeX() * terrain.SizeY();
	adjacency_list_t graph(nodeCount);
	ScalarField2D slopeField = terrain.SlopeField();
	for (int i = 0; i < terrain.SizeY(); i++)
	{
		for (int j = 0; j < terrain.SizeX(); j++)
		{
			int index1D = i * terrain.SizeY() + j;
			Vector3 v1 = terrain.Vertex(i, j);
			float w = 0.0f;
			Vector3 v;

			// Four
			// +x/-x ; +y/-y
			if (i > 0)
				AddNeighbour(graph, terrain, v1, index1D, i - 1, j);
			if (i < terrain.SizeY() - 1)
				AddNeighbour(graph, terrain, v1, index1D, i + 1, j);
			if (j > 0)
				AddNeighbour(graph, terrain, v1, index1D, i, j - 1);
			if (j < terrain.SizeX() - 1)
				AddNeighbour(graph, terrain, v1, index1D, i, j + 1);

			// Eight
			// +x/+y ; -x/+y ; +x/-y ; -x/-y
			if (neighbourType != NeighborhoodType::FOUR)
			{
				if (i > 0 && j > 0)
					AddNeighbour(graph, terrain, v1, index1D, i - 1, j - 1);
				if (i < terrain.SizeY() - 1 && j < terrain.SizeX() - 1)
					AddNeighbour(graph, terrain, v1, index1D, i + 1, j + 1);
				if (i < terrain.SizeY() - 1 && j > 0)
					AddNeighbour(graph, terrain, v1, index1D, i + 1, j - 1);
				if (i > 0 && j < terrain.SizeX() - 1)
					AddNeighbour(graph, terrain, v1, index1D, i - 1, j + 1);
			}

			// Extended
			if (neighbourType == NeighborhoodType::EXTENDED)
			{
				if (i > 1)
					AddNeighbour(graph, terrain, v1, index1D, i - 2, j);
				if (j > 1)
					AddNeighbour(graph, terrain, v1, index1D, i, j - 2);
				if (i < terrain.SizeY() - 2)
					AddNeighbour(graph, terrain, v1, index1D, i + 2, j);
				if (j < terrain.SizeX() - 2)
					AddNeighbour(graph, terrain, v1, index1D, i, j + 2);
			}
		}
	}

	std::vector<weight_t> min_distance;
	std::vector<vertex_t> previous;
	int index1DStart = iStart * terrain.SizeY() + jStart;
	int index1DEnd = iEnd * terrain.SizeY() + jEnd;

	std::cout << "Graph computed" << std::endl;
	std::cout << "Path from " << index1DStart << " to " << index1DEnd << std::endl;
	DijkstraComputePaths(index1DStart, graph, min_distance, previous);
	std::cout << "Path computed" << std::endl;

	std::list<vertex_t> path = DijkstraGetShortestPathTo(index1DEnd, previous);
	std::cout << "Path : ";
	std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
	std::cout << std::endl;
	return path;
}

GameObject* ShortestPath::GetNodeObject()
{
	Mesh* m = new Mesh(GL_TRIANGLES);
	m->ReadMesh("Data/Objs/cube.obj");
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	m->SetShader(shader);
	m->SetMaterial(Material(Color::Red(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(m);
	return obj;
}