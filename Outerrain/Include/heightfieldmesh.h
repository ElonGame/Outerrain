#pragma once
#include "mesh.h"

class HeightField;

class HeightfieldMesh : public Mesh
{
protected:
	HeightField* hf;

public:
	HeightfieldMesh(HeightField* h);

	void UpdateMeshBuffers();
};
