#pragma once

#include <memory>
#include <GL/glew.h>

#include "cameraOrbiter.h"
#include "mesh.h"
#include "gameobject.h"
#include "shader.h"
#include "material.h"
#include "component.h"

class MeshRenderer : public Component
{
protected:
	std::unique_ptr<Mesh> mesh;
	GLenum primitiveMode;
	GLuint vao;
	GLuint fullBuffer;
	GLuint indexBuffer;
	Material material;

	void UpdateBuffers();
	void RenderInternal();

public:
	MeshRenderer();
	MeshRenderer(Mesh*);
	MeshRenderer(Mesh*, const Material&);
	~MeshRenderer();

	void Render(const CameraOrbiter&);
	void CreateBuffers();
	void ClearBuffers();

	void SetMaterial(const Material& m);
	void SetPrimitiveMode(const GLuint& p);
	void SetShader(const Shader& s);

	const Mesh& GetMeshModel() const;
};
