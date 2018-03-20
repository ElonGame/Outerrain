#pragma once
#include <memory>
#include <GL/glew.h>

#include "cameraOrbiter.h"
#include "meshModel.h"
#include "gameobject.h"
#include "shader.h"
#include "materialModel.h"
#include "frame.h"

class MeshSetRenderer
{
protected:
	Mesh* mesh;
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
	MeshSetRenderer(Mesh*);
	~MeshSetRenderer();

	void AddFrame(const Frame& f);
	void ClearFrames();

	void Render(const CameraOrbiter&);

	void SetMaterial(const Material& m);
	void SetPrimitiveMode(const GLuint& p);
	void SetShader(const Shader& s);
	const Mesh& GetMeshModel() const;
};
