#pragma once
#include "meshModel.h"

class Heightfield;
class LayerField;

class HeightfieldMeshModel : public MeshModel
{
protected:
	Heightfield* hf;

public:
	HeightfieldMeshModel(Heightfield* h);

	void UpdateMeshBuffers();
};
