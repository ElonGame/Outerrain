#include "meshSet.h"

MeshSetRenderer::MeshSetRenderer()
{
	mesh = nullptr;
}

MeshSetRenderer::MeshSetRenderer(Mesh* m)
{
	mesh = nullptr;
	CreateBuffers();
}

MeshSetRenderer::~MeshSetRenderer()
{
	delete mesh;
	frames.clear();
	ClearBuffers();
}

void MeshSetRenderer::AddFrame(const Frame& f)
{
	frames.push_back(f);
}

void MeshSetRenderer::ClearFrames()
{
	frames.clear();
}

void MeshSetRenderer::Render(const CameraOrbiter& cam) 
{
	Transform mvp = cam.Projection(static_cast<float>(cam.FrameWidth()), static_cast<float>(cam.FrameHeight()), 45.0f);
	Vector3 camPos = cam.Position();
	Transform trs = frames[0].GetMatrix();
	material.SetFrameUniforms(trs, mvp, camPos);

	if (vao == 0)
		CreateBuffers();
	if (mesh->isDirty == true)
		UpdateBuffers();

	for (int i = 1; i < frames.size(); i++)
	{
		trs = frames[i].GetMatrix();
		mvp = mvp * (cam.View() * trs);
		material.shader.UniformTransform("trsMatrix", trs);
		material.shader.UniformTransform("mvpMatrix", mvp);

		glBindVertexArray(vao);
		if (mesh->indices.size() > 0)
			glDrawElements(primitiveMode, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(primitiveMode, 0, (GLsizei)mesh->vertices.size());
	}
}

void MeshSetRenderer::SetMaterial(const Material& m)
{
	material = m;
}

void MeshSetRenderer::SetPrimitiveMode(const GLuint& p)
{
	primitiveMode = p;
}

void MeshSetRenderer::SetShader(const Shader& s)
{
	material.SetShader(s);
}

const Mesh& MeshSetRenderer::GetMeshModel() const 
{
	return *mesh;
}

void MeshSetRenderer::UpdateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, fullBuffer);
	size_t offset = 0;
	size_t size = mesh->VertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->VertexBufferPtr());

	if (mesh->texcoords.size() == mesh->vertices.size())
	{
		offset = offset + size;
		size = mesh->TexcoordBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->TexcoordBufferPtr());
	}

	if (mesh->normals.size() == mesh->vertices.size())
	{
		offset = offset + size;
		size = mesh->NormalBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->NormalBufferPtr());
	}

	mesh->isDirty = false;
}

void MeshSetRenderer::CreateBuffers()
{
	if (mesh->VerticeCount() == 0)
	{
		std::cout << "Can't create buffers on empty mesh - aborting" << std::endl;
		return;
	}

	if (mesh->texcoords.size() > 0 && mesh->texcoords.size() < mesh->vertices.size())
		std::cout << "Invalid texcoord array on mesh" << std::endl;
	if (mesh->normals.size() > 0 && mesh->normals.size() < mesh->vertices.size())
		std::cout << "Invalid normal array on mesh" << std::endl;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	size_t fullSize = mesh->VertexBufferSize() + mesh->TexcoordBufferSize() + mesh->NormalBufferSize();
	glGenBuffers(1, &fullBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, fullBuffer);
	glBufferData(GL_ARRAY_BUFFER, fullSize, nullptr, GL_STATIC_DRAW);

	size_t size = 0;
	size_t offset = 0;
	size = mesh->VertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->VertexBufferPtr());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
	glEnableVertexAttribArray(0);

	if (mesh->texcoords.size() == mesh->vertices.size())
	{
		offset = offset + size;
		size = mesh->TexcoordBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->TexcoordBufferPtr());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
		glEnableVertexAttribArray(1);
	}

	if (mesh->normals.size() == mesh->vertices.size())
	{
		offset = offset + size;
		size = mesh->NormalBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh->NormalBufferPtr());
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
		glEnableVertexAttribArray(2);
	}

	if (mesh->IndexBufferSize())
	{
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->IndexBufferSize(), mesh->IndexBufferPtr(), GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	primitiveMode = GL_TRIANGLES;
}

void MeshSetRenderer::ClearBuffers()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &fullBuffer);
	glDeleteBuffers(1, &indexBuffer);
}
