#pragma once

#include <memory>
#include <GL/glew.h>

#include "gameobject.h"
#include "shader.h"
#include "material.h"
#include "component.h"

class Mesh;
class CameraOrbiter;
class Shader;

class MeshRenderer : public Component
{
protected:
	std::unique_ptr<Mesh> mesh;
	GLenum primitiveMode;
	GLuint vao;
	GLuint fullBuffer;
	GLuint indexBuffer;
	MaterialBase* material;

	size_t vertexCount;
	size_t triangleCount;

	void UpdateBuffers();
	void RenderInternal();

public:
	MeshRenderer();
	MeshRenderer(Mesh*);
	MeshRenderer(Mesh*, MaterialBase*);
	~MeshRenderer();

	void Render(const CameraOrbiter&);
	void CreateBuffers();
	void ClearBuffers();

	void SetMaterial(MaterialBase* m);
	void SetShader(const Shader& s);

	const Mesh& GetMesh() const;
};
