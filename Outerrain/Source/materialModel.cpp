#include "materialModel.h"


MaterialModel::MaterialModel()
{
	texture0 = 0;
	albedo = Color(0.0f);
	shininess = 0.0f;
}

void MaterialModel::SetUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos)
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
}

void MaterialModel::SetShader(const Shader& s)
{
	shader = s;
}

MaterialModel MaterialModel::DefaultDiffuse()
{
	MaterialModel ret;
	ret.albedo = Color::Grey();
	ret.shininess = 0.0f;
	ret.shader.InitFromFile("Shaders/TerrainShader.glsl");
	return ret;
}