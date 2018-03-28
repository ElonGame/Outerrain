#pragma once
#include <memory>
#include <GL/glew.h>
#include <vector>

#include "box.h"
#include "material.h"
#include "frame.h"

class Mesh;
class CameraOrbiter;
class Shader;

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
	MeshSetRenderer(Mesh*, const std::vector<Frame>&);
	~MeshSetRenderer();

	void AddFrame(const Frame& f);
	void ClearFrames();
	int GetFrameCount() const;
	Box GetBounds() const;
	void Render(const CameraOrbiter&);

	void SetMaterial(const Material& m);
	void SetPrimitiveMode(const GLuint& p);
	void SetShader(const Shader& s);
	const Mesh& GetMeshModel() const;
};
