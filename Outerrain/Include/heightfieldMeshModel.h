#pragma once
#include "meshModel.h"

class Heightfield;
class LayerField;

class HeightfieldMeshModel : public MeshModel
{
protected:
	Heightfield* hf;
	LayerField* lf;

	void UpdateMeshBuffersHeightfield();
	void UpdateMeshBuffersLayerField();

public:
	HeightfieldMeshModel(Heightfield* h);
	HeightfieldMeshModel(LayerField* h);

	void UpdateMeshBuffers();
};
