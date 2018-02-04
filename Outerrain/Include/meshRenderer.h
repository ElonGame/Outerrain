#pragma once

#include <memory>
#include <GL/glew.h>

#include "cameraOrbiter.h"
#include "meshModel.h"
#include "gameobject.h"
#include "shader.h"
#include "materialModel.h"
#include "component.h"

class MeshRenderer : public Component
{
private:
	std::unique_ptr<MeshModel> mesh;

	GLenum primitiveMode;
	GLuint vao;
	GLuint fullBuffer;
	GLuint indexBuffer;

	MaterialModel material;

	void UpdateBuffers();
	void RenderInternal();

public:
	MeshRenderer();
	MeshRenderer(MeshModel*);
	MeshRenderer(MeshModel*, const MaterialModel&);
	~MeshRenderer();

	void Render(const CameraOrbiter&);
	void CreateBuffers();
	void ClearBuffers();

	void SetMaterial(const MaterialModel& m);
	void SetPrimitiveMode(const GLuint& p);
	void SetShader(const Shader& s);

	const MeshModel& GetMeshModel() const;
};
