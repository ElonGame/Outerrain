#include "material.h"
#include "texture2D.h"
#include "mainwindow.h"


Material::Material()
{
	texture0 = texture1 = texture2 = texture3 = 0;
	albedo = Color(0.0f);
	shininess = 0.0f;
}

Material::Material(const std::string& shaderPath, const Color& alb, double shin)
{
	shader = Shader(shaderPath);
	albedo = alb;
	shininess = shin;
	texture0 = texture1 = texture2 = texture3 = 0;
}

void Material::SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos)
{
	shader.Attach();

	shader.UniformVec3("lightDir", MainWindow::sceneLight.Direction());
	shader.UniformColor("lightColor", MainWindow::sceneLight.GetColor());
	shader.UniformColor("lightAmbientColor", MainWindow::sceneLight.AmbientColor());
	shader.UniformFloat("lightStrength", MainWindow::sceneLight.Strength());

	shader.UniformTransform("trsMatrix", trs);
	shader.UniformTransform("mvpMatrix", mvp);
	shader.UniformVec3("camPos", camPos);

	shader.UniformColor("albedo", albedo);
	shader.UniformFloat("shininess", shininess);
	shader.UniformInt("shaderType", shaderType);

	shader.UniformInt("useTexture", texture0 != 0 ? 1 : 0);
	if (texture0 != 0)
		shader.UniformTexture("texture0", 0, texture0);
	if (texture1 != 0)
		shader.UniformTexture("texture1", 1, texture1);
	if (texture2 != 0)
		shader.UniformTexture("texture2", 2, texture2);
	if (texture3 != 0)
		shader.UniformTexture("texture3", 3, texture3);
}

void Material::SetFrameUniforms(const Transform& mvp, const Vector3 camPos)
{
	shader.Attach();

	shader.UniformVec3("lightDir", MainWindow::sceneLight.Direction());
	shader.UniformColor("lightColor", MainWindow::sceneLight.GetColor());
	shader.UniformColor("lightAmbientColor", MainWindow::sceneLight.AmbientColor());
	shader.UniformFloat("lightStrength", MainWindow::sceneLight.Strength());

	shader.UniformTransform("mvpMatrix", mvp);
	shader.UniformVec3("camPos", camPos);

	shader.UniformColor("albedo", albedo);
	shader.UniformFloat("shininess", shininess);
	shader.UniformInt("shaderType", shaderType);

	shader.UniformInt("useTexture", texture0 != 0 ? 1 : 0);
	if (texture0 != 0)
		shader.UniformTexture("texture0", 0, texture0);
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
Material Material::DiffuseMat;
Material Material::TerrainSplatmapMat;
Material Material::SimpleTexturedMat;
Material Material::WireframeMat;
Material Material::NormalMat;

void Material::InitStaticMaterials()
{
	// Diffuse
	DiffuseMat.shader = Shader("Shaders/Diffuse.glsl");
	DiffuseMat.albedo = Color::Grey();
	DiffuseMat.shaderType = ShaderType::DiffuseShader;
	DiffuseMat.shininess = 0.0f;
	DiffuseMat.texture0 = DiffuseMat.texture1 = DiffuseMat.texture2 = DiffuseMat.texture3 = 0;

	// Terrain splatmap
	TerrainSplatmapMat.shader = Shader("Shaders/TerrainSplatmap.glsl");
	TerrainSplatmapMat.albedo = Color::White();
	TerrainSplatmapMat.shininess = 0.0f;
	TerrainSplatmapMat.shaderType = ShaderType::TerrainSplatmapShader;
	TerrainSplatmapMat.texture0 = Texture2D::MakeGLTexture("Data/Textures/grass.png", 0, true);
	TerrainSplatmapMat.texture1 = Texture2D::MakeGLTexture("Data/Textures/sand.png", 1, true);
	TerrainSplatmapMat.texture2 = Texture2D::MakeGLTexture("Data/Textures/rock.png", 3, true);
	TerrainSplatmapMat.texture3 = Texture2D::MakeGLTexture("Data/Textures/snow.png", 4, true);

	// Simple Textured
	SimpleTexturedMat.shader = Shader("Shaders/SimpleTextured.glsl");
	SimpleTexturedMat.albedo = Color::Grey();
	SimpleTexturedMat.shininess = 0.0f;
	SimpleTexturedMat.shaderType = ShaderType::SimpleTexturedShader;
	SimpleTexturedMat.texture0 = TerrainSplatmapMat.texture0;
	SimpleTexturedMat.texture1 = SimpleTexturedMat.texture2 = SimpleTexturedMat.texture3 = 0;

	// Wireframe
	WireframeMat.shader = Shader("Shaders/Wireframe.glsl");
	WireframeMat.albedo = Color::Green();
	WireframeMat.shininess = 0.0f;
	WireframeMat.shaderType = ShaderType::WireframeShader;
	WireframeMat.texture0 = WireframeMat.texture1 = WireframeMat.texture2 = WireframeMat.texture3 = 0;

	// Normal
	NormalMat.shader = Shader("Shaders/Normal.glsl");
	NormalMat.albedo = Color::Green();
	NormalMat.shininess = 0.0f;
	NormalMat.shaderType = ShaderType::NormalShader;
	NormalMat.texture0 = WireframeMat.texture1 = WireframeMat.texture2 = WireframeMat.texture3 = 0;
}

void Material::ReleaseStaticMaterials()
{
	DiffuseMat.Release();
	SimpleTexturedMat.Release();
	TerrainSplatmapMat.Release();
	WireframeMat.Release();
	NormalMat.Release();
}
