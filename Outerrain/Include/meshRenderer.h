#pragma once

#include <memory>
#include <GL/glew.h>

#include "meshModel.h"

class MeshRenderer
{
private:
	std::unique_ptr<MeshModel> mesh;

	GLenum primitiveMode;
	GLuint vao;
	GLuint fullBuffer;
	GLuint indexBuffer;

public:
	MeshRenderer();
	MeshRenderer(MeshModel*);
	~MeshRenderer();

	void CreateBuffers();
	void ClearBuffers();
	void SetPrimitiveMode(const GLuint& p);

	const MeshModel& GetMeshModel() const { return *mesh.get(); }
};
