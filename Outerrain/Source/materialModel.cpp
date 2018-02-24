#include "materialModel.h"
#include "image.h"

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
		shader.UniformInt("useTexture0", 1);
		shader.UniformTexture("texture0", 0, texture0);
	}
	else
		shader.UniformInt("useTexture0", 0);

	if (texture1 != 0)
	{
		shader.UniformInt("useTexture1", 1);
		shader.UniformTexture("texture1", 0, texture1);
	}
	else
		shader.UniformInt("useTexture1", 0);

	if (texture2 != 0)
	{
		shader.UniformInt("useTexture2", 1);
		shader.UniformTexture("texture2", 0, texture2);
	}
	else
		shader.UniformInt("useTexture2", 0);

	if (texture3 != 0)
	{
		shader.UniformInt("useTexture3", 1);
		shader.UniformTexture("texture3", 0, texture3);
	}
	else
		shader.UniformInt("useTexture3", 0);
}

void MaterialModel::SetShader(const Shader& s)
{
	shader = s;
}

/* Static Materials */
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
	
	// Terrain textures
	ret.texture0 = Image("Data/Textures/grass.png").GetGLTexture(0, GL_RGBA);
	ret.texture1 = Image("Data/Textures/sand.png").GetGLTexture(1, GL_RGBA);
	ret.texture2 = Image("Data/Textures/rock.png").GetGLTexture(2, GL_RGBA);
	ret.texture3 = Image("Data/Textures/snow.png").GetGLTexture(3, GL_RGBA);

	return ret;
}
