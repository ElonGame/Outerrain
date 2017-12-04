#include "mesh.h"
#include "cameraOrbiter.h"
#include "shader.h"
#include <algorithm>
#include <cassert>


Mesh::Mesh() :
	vertices(), texcoords(), normals(), colors(), indices(),
	primitiveDrawn(GL_TRIANGLES), VAO(0), fullBuffer(0), indexBuffer(0),
	shader() { }

void Mesh::AddVertex(const Vector3& v)
{
	vertices.push_back(v);
	updateBuffersNextDraw = true;
}

void Mesh::AddVertex(const int& i, const Vector3& v)
{
	assert(i < vertices.size());
	vertices[i] = v;
	updateBuffersNextDraw = true;
}

void Mesh::AddNormal(const Vector3& n)
{
	normals.push_back(n);
	updateBuffersNextDraw = true;
}

void Mesh::AddNormal(const int& i, const Vector3& v)
{
	assert(i < normals.size());
	normals[i] = v;
	updateBuffersNextDraw = true;
}

void Mesh::AddColor(const Vector4& v)
{
	colors.push_back(v);
	updateBuffersNextDraw = true;
}

void Mesh::AddColor(const int& i, const Vector4& v)
{
	assert(i < colors.size());
	updateBuffersNextDraw = true;
	colors[i] = v;
}

void Mesh::AddTriangle(const unsigned int a, const unsigned int b, const unsigned int c)
{
	assert(a < vertices.size());
	assert(b < vertices.size());
	assert(c < vertices.size());
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
	updateBuffersNextDraw = true;
}

void Mesh::AddTexcoord(const Vector2& t)
{
	texcoords.push_back(t);
	updateBuffersNextDraw = true;
}

void Mesh::AddTexcoord(const int& i, const Vector2& t)
{
	assert(i < texcoords.size());
	updateBuffersNextDraw = true;
	texcoords[i] = t;
}

void Mesh::SetShader(const Shader& s)
{
	shader = s;
}

Vector3 Mesh::Vertex(int i) const
{
	return vertices[i];
}

Vector3 Mesh::Normal(int i) const
{
	return normals[i];
}

Vector2 Mesh::Texcoord(int i) const
{
	return texcoords[i];
}

Vector4 Mesh::Color(int i) const
{
	return colors[i];
}

Bounds Mesh::GetBounds() const
{
	Bounds ret;
	if (vertices.size() < 1)
		return ret;
	ret.min = Vector3(vertices[0]);
	ret.max = ret.min;
	for (unsigned int i = 1; i < (unsigned int)vertices.size(); i++)
	{
		Vector3 p = vertices[i];
		ret.min = Vector3(std::min(ret.min.x, p.x), std::min(ret.min.y, p.y), std::min(ret.min.z, p.z));
		ret.max = Vector3(std::max(ret.max.x, p.x), std::max(ret.max.y, p.y), std::max(ret.max.z, p.z));
	}
	return ret;
}

void Mesh::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &fullBuffer);
	glDeleteBuffers(1, &indexBuffer);
	shader.Release();
}

void Mesh::CreateBuffers(const bool use_texcoord, const bool use_normal, const bool use_color)
{
	if (vertices.size() == 0)
		return;

	if (use_texcoord && texcoords.size() > 0 && texcoords.size() < vertices.size())
		printf("[error] invalid texcoords array...\n");
	if (use_normal && normals.size() > 0 && normals.size() < vertices.size())
		printf("[error] invalid normals array...\n");
	if (use_color && colors.size() > 0 && colors.size() < vertices.size())
		printf("[error] invalid colors array...\n");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	size_t size = VertexBufferSize() + TexcoordBufferSize() + NormalBufferSize() + ColorBufferSize();
	glGenBuffers(1, &fullBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, fullBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	size_t offset = 0;
	size = VertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, VertexBufferPtr());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
	glEnableVertexAttribArray(0);

	if (texcoords.size() == vertices.size() && use_texcoord)
	{
		offset = offset + size;
		size = TexcoordBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, TexcoordBufferPtr());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
		glEnableVertexAttribArray(1);
	}

	if (normals.size() == vertices.size() && use_normal)
	{
		offset = offset + size;
		size = NormalBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, NormalBufferPtr());
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
		glEnableVertexAttribArray(2);
	}

	if (colors.size() == vertices.size() && use_color)
	{
		offset = offset + size;
		size = ColorBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, ColorBufferPtr());
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (const void *)offset);
		glEnableVertexAttribArray(3);
	}

	if (IndexBufferSize())
	{
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize(), IndexBufferPtr(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::UpdateBuffers(const bool use_texcoord, const bool use_normal, const bool use_color)
{
	assert(VAO > 0);
	assert(fullBuffer > 0);
	if (updateBuffersNextDraw == false)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, fullBuffer);
	size_t offset = 0;
	size_t size = VertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, VertexBufferPtr());

	if (texcoords.size() == vertices.size() && use_texcoord)
	{
		offset = offset + size;
		size = TexcoordBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, TexcoordBufferPtr());
	}

	if (normals.size() == vertices.size() && use_normal)
	{
		offset = offset + size;
		size = NormalBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, NormalBufferPtr());
	}

	if (colors.size() == vertices.size() && use_color)
	{
		offset = offset + size;
		size = ColorBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, ColorBufferPtr());
	}

	updateBuffersNextDraw = false;
}

void Mesh::SetNormals()
{
	// Normale d une face
	Vector3 normale;

	// Norme d une normale
	double Norme;

	// Indices des sommets d une face
	int a, b, c;

	/* Parcours des normales des sommets du maillage */
	if (normals.size() == 0)
	{
		for (unsigned int i = 0; i < vertices.size(); ++i)
		{
			// Initialisation des normales
			normals.push_back(Vector3(0, 0, 0));
		}
	}
	for (unsigned int i = 0; i < vertices.size(); ++i)
		normals[i] = Vector3(0, 0, 0);

	/* Parcours des faces du maillage */
	for (unsigned int i = 0; i < indices.size(); ++i)
	{
		// Sommets a, b, c de la face
		a = indices[3 * i];
		b = indices[(3 * i) + 1];
		c = indices[(3 * i) + 2];

		// Calcul de la normale de la face
		Vector3 AB, AC;

		/* Coordonnee du vecteur AB */
		AB = (vertices[b] - vertices[a]);

		/* Coordonnee du vecteur AC */
		AC = (vertices[c] - vertices[a]);

		/* Coordonnees de la normale */
		normale = Cross(AB, AC);

		// Modification des normales des sommets de la face
		// Normale du sommet a
		normals[a] = normals[a] + normale;
		normals[b] = normals[b] + normale;
		normals[c] = normals[c] + normale;

	}//for_i

	 /* Parcours des normales des sommets */
	float norme;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		norme = Magnitude(normals[i]);

		if (norme != 0)
			normals[i] = normals[i] * 1.0 / norme;

	}
}

void Mesh::Draw()
{
	if (VAO == 0)
		CreateBuffers(true, true, true);
	assert(VAO != 0);

	if (updateBuffersNextDraw == true)
		UpdateBuffers(true, true, true);

	glBindVertexArray(VAO);
	glUseProgram(shader.GetProgram());

	if (indices.size() > 0)
		glDrawElements(primitiveDrawn, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(primitiveDrawn, 0, (GLsizei)vertices.size());
	glBindVertexArray(0);
}

void Mesh::Draw(const CameraOrbiter& orbiter)
{
	GLuint m_program = shader.GetProgram();
	assert(m_program != 0);
	glUseProgram(m_program);

	Transform mv = orbiter.View() * Identity();
	Transform mvp = orbiter.Projection(orbiter.FrameWidth(), orbiter.FrameHeight(), 45) * mv;

	shader.UniformTransform("mvMatrix", mv);
	shader.UniformTransform("mvpMatrix", mvp);

	Draw();
}

void Mesh::ReadMesh(const char *filename)
{
	FILE *in = fopen(filename, "rt");

	printf("loading mesh '%s'...\n", filename);

	std::vector<Vector3> vert;
	std::vector<Vector2> tex;
	std::vector<Vector3> norm;

	std::vector<int> idp;
	std::vector<int> idt;
	std::vector<int> idn;

	char line_buffer[1024];
	bool error = true;
	for (;;)
	{
		// charge une ligne du fichier
		if (fgets(line_buffer, sizeof(line_buffer), in) == NULL)
		{
			error = false;       // fin du fichier, pas d'erreur detectee
			break;
		}

		// force la fin de la ligne, au cas ou
		line_buffer[sizeof(line_buffer) - 1] = 0;

		// saute les espaces en debut de ligne
		char *line = line_buffer;
		while (*line && isspace(*line))
			line++;

		if (line[0] == 'v')
		{
			float x, y, z;
			if (line[1] == ' ')          // position x y z
			{
				if (sscanf(line, "v %f %f %f", &x, &y, &z) != 3)
					break;
				vert.push_back(Vector3(x, y, z));
			}
			else if (line[1] == 'n')     // normal x y z
			{
				if (sscanf(line, "vn %f %f %f", &x, &y, &z) != 3)
					break;
				norm.push_back(Vector3(x, y, z));
			}
			else if (line[1] == 't')     // texcoord x y
			{
				if (sscanf(line, "vt %f %f", &x, &y) != 2)
					break;
				tex.push_back(Vector2(x, y));
			}
		}

		else if (line[0] == 'f')         // triangle a b c, les sommets sont numerotes a partir de 1 ou de la fin du tableau (< 0)
		{
			idp.clear();
			idt.clear();
			idn.clear();

			int next;
			for (line = line + 1; ; line = line + next)
			{
				idp.push_back(0);
				idt.push_back(0);
				idn.push_back(0);         // 0: invalid index

				next = 0;
				if (sscanf(line, " %d/%d/%d %n", &idp.back(), &idt.back(), &idn.back(), &next) == 3)
					continue;
				else if (sscanf(line, " %d/%d %n", &idp.back(), &idt.back(), &next) == 2)
					continue;
				else if (sscanf(line, " %d//%d %n", &idp.back(), &idn.back(), &next) == 2)
					continue;
				else if (sscanf(line, " %d %n", &idp.back(), &next) == 1)
					continue;
				else if (next == 0)      // fin de ligne
					break;
			}

			for (int v = 2; v + 1 < (int)idp.size(); v++)
			{
				int idv[3] = { 0, v - 1, v };
				for (int i = 0; i < 3; i++)
				{
					int k = idv[i];
					int p = (idp[k] < 0) ? (int)vert.size() + idp[k] : idp[k] - 1;
					int t = (idt[k] < 0) ? (int)tex.size() + idt[k] : idt[k] - 1;
					int n = (idn[k] < 0) ? (int)norm.size() + idn[k] : idn[k] - 1;

					if (p < 0) break; // error
					if (t >= 0) AddTexcoord(tex[t]);
					if (n >= 0) AddNormal(norm[n]);
					AddVertex(vert[p]);
				}
			}
		}
	}

	fclose(in);

	if (error)
		printf("loading mesh '%s'...\n[error]\n%s\n\n", filename, line_buffer);
}