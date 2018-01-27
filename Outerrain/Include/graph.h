#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>

#include "terrain.h"
#include "gameobject.h"
#include "mesh.h"


/* Minimalist graph structure as an Adjacency List. */
typedef int vertex_t;
typedef double weight_t;
enum NeighborhoodType
{
	FOUR = 1,
	EIGHT = 2,
	EXTENDED = 3
};
struct neighbor 
{
    vertex_t target;
    weight_t weight;

    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) 
		{ 
		}
};
typedef std::vector<std::vector<neighbor>> adjacency_list_t;

/* Public Interface for road generation (and possibly rivers later) */
class ShortestPath
{
public:
	static void DijkstraComputePaths(vertex_t source,
                          const adjacency_list_t &adjacency_list,
                          std::vector<weight_t> &min_distance,
                          std::vector<vertex_t> &previous);

	static std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, 
														 const std::vector<vertex_t> &previous);
	static std::list<vertex_t> FindPath(const VegetationTerrain& terrain, int iStart, int jStart, int iEnd, int jEnd, NeighborhoodType);
	static GameObject* GetNodeObject();
};