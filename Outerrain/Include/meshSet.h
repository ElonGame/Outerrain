#pragma once
#include <memory>
#include <vector>

#include "meshModel.h"
#include "frame.h"
#include "cameraOrbiter.h"
#include "materialModel.h"

class MeshSetRenderer
{
protected:
	std::unique_ptr<Mesh> mesh;
	std::vector<Frame> frames;

	GLenum primitiveMode;
	GLuint vao;
	GLuint fullBuffer;
	GLuint indexBuffer;
	Material material;

	void CreateBuffers();
	void UpdateBuffers();
	void ClearBuffers();

public:
	MeshSetRenderer();
	~MeshSetRenderer();

	void AddFrame(const Frame& f);
	void ClearFrames();

	void Render(const CameraOrbiter&);

	void SetMaterial(const Material& m);
	void SetPrimitiveMode(const GLuint& p);
	void SetShader(const Shader& s);
	const Mesh& GetMeshModel() const;
};
