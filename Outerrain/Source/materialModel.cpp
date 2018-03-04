#include "materialModel.h"
#include "image.h"
#include "texture.h"


MaterialModel::MaterialModel()
{
	texture0 = texture1 = texture2 = texture3 = 0;
	albedo = Color(0.0f);
	shininess = 0.0f;
}

void MaterialModel::SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos)
{
	shader.Attach();

	shader.UniformTransform("trsMatrix", trs);
	shader.UniformTransform("mvpMatrix", mvp);
	shader.UniformVec3("camPos", camPos);

	shader.UniformColor("albedo", albedo);
	shader.UniformFloat("shininess", shininess);
	shader.UniformInt("shaderType", shaderType);

	if (texture0 != 0)
	{
		shader.UniformInt("useTexture", 1);
		shader.UniformTexture("texture0", 0, texture0);
	}
	else
		shader.UniformInt("useTexture", 0);

	if (texture1 != 0)
		shader.UniformTexture("texture1", 1, texture1);

	if (texture2 != 0)
		shader.UniformTexture("texture2", 2, texture2);

	if (texture3 != 0)
		shader.UniformTexture("texture3", 3, texture3);
}

void MaterialModel::Release()
{
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteTextures(1, &texture3);
}

void MaterialModel::SetShader(const Shader& s)
{
	shader = s;
}

/* Static Materials */
MaterialModel MaterialModel::DefaultDiffuseMat;
MaterialModel MaterialModel::TerrainTexturedMat;
MaterialModel MaterialModel::DefaultTexturedMat;

void MaterialModel::InitStaticMaterials()
{
	// Diffuse Grey
	DefaultDiffuseMat.albedo = Color::Grey();
	DefaultDiffuseMat.shaderType = ShaderType::DiffuseGrey;
	DefaultDiffuseMat.shininess = 0.0f;
	DefaultDiffuseMat.shader.InitFromFile("Shaders/TerrainShader.glsl");
	DefaultDiffuseMat.texture0 = DefaultDiffuseMat.texture1 = DefaultDiffuseMat.texture2 = DefaultDiffuseMat.texture3 = 0;

	// Terrain splatmap
	TerrainTexturedMat.albedo = Color::White();
	TerrainTexturedMat.shininess = 0.0f;
	TerrainTexturedMat.shaderType = ShaderType::TerrainSplatmap;
	TerrainTexturedMat.shader.InitFromFile("Shaders/TerrainShader.glsl");
	TerrainTexturedMat.texture0 = ReadTexture(0, "Data/Textures/grass.png", GL_RGBA);
	TerrainTexturedMat.texture1 = ReadTexture(1, "Data/Textures/sand.png", GL_RGBA);
	TerrainTexturedMat.texture2 = ReadTexture(2, "Data/Textures/rock.png", GL_RGBA);
	TerrainTexturedMat.texture3 = ReadTexture(3, "Data/Textures/snow.png", GL_RGBA);

	// Simple Textured
	DefaultTexturedMat.albedo = Color::Grey();
	DefaultTexturedMat.shininess = 0.0f;
	DefaultTexturedMat.shaderType = ShaderType::SimpleTextured;
	DefaultTexturedMat.shader.InitFromFile("Shaders/TerrainShader.glsl");
	DefaultTexturedMat.texture0 = ReadTexture(0, "Data/Textures/grass.png", GL_RGBA);;
	DefaultTexturedMat.texture1 = DefaultTexturedMat.texture2 = DefaultTexturedMat.texture3 = 0;
}
