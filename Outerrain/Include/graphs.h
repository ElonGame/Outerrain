#pragma once
#include "vec.h"
#include <vector>

// Minimalist graph structure
// Neighbours are represented by the following mapping.
// A neighbour i exists if and only if W[i] != -1.0f
// 0 => i - 1, j
// 1 => i + 1, j
// 2 => i, j - 1
// 3 => i, j + 1
// 4 => i - 1, j - 1
// 5 => i - 1, j + 1
// 6 => i + 1, j - 1
// 7 => i + 1, j + 1

#define ADJ_SIZE 8
#define NULL_VALUE -1.0f
struct GraphNode
{
public:
	float W[ADJ_SIZE];

	GraphNode()
	{
		for (int i = 0; i < ADJ_SIZE; i++)
			W[i] = NULL_VALUE;
	}
};
