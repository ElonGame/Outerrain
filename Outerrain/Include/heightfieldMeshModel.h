#pragma once
#include "meshModel.h"

class Heightfield;
class LayerField;

class HeightfieldMesh : public Mesh
{
protected:
	Heightfield* hf;

public:
	HeightfieldMesh(Heightfield* h);

	void UpdateMeshBuffers();
};
