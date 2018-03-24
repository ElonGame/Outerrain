#include "material.h"
#include "texture2D.h"


Material::Material()
{
	texture0 = texture1 = texture2 = texture3 = 0;
	albedo = Color(0.0f);
	shininess = 0.0f;
}

void Material::SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos)
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

void Material::Release()
{
	if (shader.GetProgram() != 0)
		glDeleteProgram(shader.GetProgram());
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteTextures(1, &texture3);
}

bool Material::operator==(const Material& m)
{
	if (albedo.r != m.albedo.r || albedo.g != m.albedo.g || albedo.b != m.albedo.b || albedo.a != m.albedo.a)
		return false;
	if (shininess != m.shininess)
		return false;
	if (texture0 != m.texture0)
		return false;
	if (texture1 != m.texture1)
		return false;
	if (texture2 != m.texture2)
		return false;
	if (texture3 != m.texture3)
		return false;
	if (shaderType != m.shaderType)
		return false;
	return true;
}

void Material::SetShader(const Shader& s)
{
	shader = s;
}

/* Static Materials */
Material Material::DefaultDiffuseMat;
Material Material::TerrainTexturedMat;
Material Material::DefaultTexturedMat;
Material Material::WireframeMat;

void Material::InitStaticMaterials()
{
	// Diffuse Grey
	DefaultDiffuseMat.albedo = Color::Grey();
	DefaultDiffuseMat.shaderType = ShaderType::DiffuseGrey;
	DefaultDiffuseMat.shininess = 0.0f;
	DefaultDiffuseMat.shader = Shader("Shaders/TerrainShader.glsl");
	DefaultDiffuseMat.texture0 = DefaultDiffuseMat.texture1 = DefaultDiffuseMat.texture2 = DefaultDiffuseMat.texture3 = 0;

	// Terrain splatmap
	TerrainTexturedMat.albedo = Color::White();
	TerrainTexturedMat.shininess = 0.0f;
	TerrainTexturedMat.shaderType = ShaderType::TerrainSplatmap;
	TerrainTexturedMat.shader = DefaultDiffuseMat.shader; //Shader("Shaders/TerrainShader.glsl");
	TerrainTexturedMat.texture0 = Texture2D::MakeGLTexture("Data/Textures/grass.png", 0, true);
	TerrainTexturedMat.texture1 = Texture2D::MakeGLTexture("Data/Textures/sand.png", 1, true);
	TerrainTexturedMat.texture2 = Texture2D::MakeGLTexture("Data/Textures/rock.png", 3, true);
	TerrainTexturedMat.texture3 = Texture2D::MakeGLTexture("Data/Textures/snow.png", 4, true);

	// Simple Textured
	DefaultTexturedMat.albedo = Color::Grey();
	DefaultTexturedMat.shininess = 0.0f;
	DefaultTexturedMat.shaderType = ShaderType::SimpleTextured;
	DefaultTexturedMat.shader = DefaultDiffuseMat.shader;
	DefaultTexturedMat.texture0 = TerrainTexturedMat.texture0;
	DefaultTexturedMat.texture1 = DefaultTexturedMat.texture2 = DefaultTexturedMat.texture3 = 0;

	// Wireframe
	WireframeMat.albedo = Color::Green();
	WireframeMat.shininess = 0.0f;
	WireframeMat.shaderType = ShaderType::Wireframe;
	WireframeMat.shader = DefaultDiffuseMat.shader;
	WireframeMat.texture0 = WireframeMat.texture1 = WireframeMat.texture2 = WireframeMat.texture3 = 0;
}

void Material::ReleaseStaticMaterials()
{
	DefaultDiffuseMat.Release();
	DefaultTexturedMat.Release();
	TerrainTexturedMat.Release();
}
