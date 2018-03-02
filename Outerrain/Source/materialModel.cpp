#include "materialModel.h"
#include "image.h"
#include "texture.h"


MaterialModel::MaterialModel()
{
	texture0 = 0;
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

void MaterialModel::SetShader(const Shader& s)
{
	shader = s;
}

/* Static Materials */
MaterialModel MaterialModel::DefaultDiffuseMat;
MaterialModel MaterialModel::TerrainTexturedMat;

void MaterialModel::InitStaticMaterials()
{
	DefaultDiffuseMat = DefaultDiffuse();
	TerrainTexturedMat = TerrainTextured();
}

MaterialModel MaterialModel::DefaultDiffuse()
{
	MaterialModel ret;
	ret.albedo = Color::Grey();
	ret.shininess = 0.0f;
	ret.shader.InitFromFile("Shaders/TerrainShader.glsl");
	ret.texture0 = ret.texture1 = ret.texture2 = ret.texture3 = 0;
	return ret;
}

MaterialModel MaterialModel::TerrainTextured()
{
	MaterialModel ret;
	ret.albedo = Color::White();
	ret.shininess = 0.0f;
	ret.shader.InitFromFile("Shaders/TerrainShader.glsl");
	
	ret.texture0 = ReadTexture(0, "Data/Textures/grass.png", GL_RGBA);
	ret.texture1 = ReadTexture(1, "Data/Textures/sand.png", GL_RGBA);
	ret.texture2 = ReadTexture(2, "Data/Textures/rock.png", GL_RGBA);
	ret.texture3 = ReadTexture(3, "Data/Textures/snow.png", GL_RGBA);

	return ret;
}
