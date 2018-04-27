#include "renderlib.h"

// Buffers
template<typename T>
void GenericBuffer<T>::Allocate()
{
	glGenBuffers(1, &bufferIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * buffer.size(), &buffer.front(), GL_STATIC_DRAW);
}

void VertexBuffer::Allocate()
{
	int bufferSize = buffer.size();
	size_t vertexBufferSize = bufferSize * sizeof(Vector3);
	size_t texcoordBufferSize = bufferSize * sizeof(Vector2);
	size_t normalBufferSize = bufferSize * sizeof(Vector3);
	size_t fullSize = vertexBufferSize + texcoordBufferSize + normalBufferSize;
	size_t offset = 0;

	glGenBuffers(1, &bufferIndex);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, fullSize, nullptr, GL_STATIC_DRAW);

	size_t size = 0;
	size_t offset = 0;
	size = vertexBufferSize;
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

	Unbind();
}


// MeshRender
MeshRender::MeshRender(const Mesh& m)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::vector<VertexInfo> vertexInfos;
	auto vertexData = m.Vertices();
	auto normalData = m.Normals();
	auto texcoordData = m.Texcoords();
	for (int i = 0; i < vertexData.size(); i++)
	{
		VertexInfo info;
		info.position = vertexData[i];
		info.normal = normalData[i];
		info.texcoord = texcoordData[i];
		vertexInfos.push_back(info);
	}
	vertexBuffer = VertexBuffer(vertexInfos);
	indexBuffer = GenericBuffer<unsigned int>(m.Indices());

	glBindVertexArray(0);
}

MeshRender::~MeshRender()
{
	glDeleteVertexArrays(1, &vao);
}

void MeshRender::Draw(const CameraOrbiter& cam)
{
	Transform trs = Identity();
	Transform mvp = cam.Projection() * (cam.View() * trs);
	Vector3 camPos = cam.Position();

	mat.SetFrameUniforms(trs, mvp, camPos);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer.GetBufferSize(), GL_UNSIGNED_INT, 0);
}