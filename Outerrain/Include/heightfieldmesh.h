#pragma once
#include "mesh.h"

class Heightfield;

class HeightfieldMesh : public Mesh
{
protected:
	Heightfield* hf;

public:
	HeightfieldMesh(Heightfield* h);

	void UpdateMeshBuffers();
};
