#include "meshRenderer.h"
#include "mesh.h"
#include "shader.h"
#include "camera-orbiter.h"
#include "app-stats.h"
#include <iostream>


MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* m)
{
	mesh = std::unique_ptr<Mesh>(m);
	CreateBuffers();
}

MeshRenderer::MeshRenderer(Mesh* m, MaterialBase* mat) : MeshRenderer(m)
{
	material = mat;
}

MeshRenderer::~MeshRenderer()
{
	AppStatistics::vertexCount -= vertexCount;
	AppStatistics::triangleCount -= triangleCount;
	mesh.release();
	ClearBuffers();
}

void MeshRenderer::UpdateBuffers()
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

void MeshRenderer::RenderInternal()
{
	if (vao == 0)
		CreateBuffers();

	glBindVertexArray(vao);
	if (mesh->isDirty == true)
		UpdateBuffers();
	if (mesh->indices.size() > 0)
		glDrawElements(primitiveMode, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(primitiveMode, 0, (GLsizei)mesh->vertices.size());
}

void MeshRenderer::Render(const CameraOrbiter& cam)
{
	if (mesh == nullptr)
		return;
	Transform trs = gameObject->GetObjectToWorldMatrix();
	Transform mvp = cam.Projection() * (cam.ViewDirection() * trs);
	Vector3 camPos = cam.Position();
	if (material->shaderType == MaterialType::WireframeMaterial)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	material->SetFrameUniforms(trs, mvp, camPos);
	RenderInternal();
}

void MeshRenderer::CreateBuffers()
{
	if (mesh->VertexCount() == 0)
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

	if (mesh->IndexBufferSize() > 0)
	{
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->IndexBufferSize(), mesh->IndexBufferPtr(), GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	primitiveMode = GL_TRIANGLES;

	// Records stats
	triangleCount = mesh->TriangleCount();
	vertexCount = mesh->VertexCount();
	AppStatistics::vertexCount += vertexCount;
	AppStatistics::triangleCount += triangleCount;
}

void MeshRenderer::ClearBuffers()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &fullBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void MeshRenderer::SetMaterial(MaterialBase* m)
{
	material = m;
}

void MeshRenderer::SetShader(const Shader& s)
{
	material->SetShader(s);
}

const Mesh& MeshRenderer::GetMesh() const
{
	return *mesh.get();
}
