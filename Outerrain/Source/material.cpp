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
Material Material::DiffuseMaterialInstance;
Material Material::TerrainSplatmapMaterialInstance;
Material Material::TexturedMaterialInstance;
Material Material::WireframeMaterialInstance;
Material Material::NormalMaterialInstance;

void Material::InitStaticMaterials()
{
	// Diffuse
	DiffuseMaterialInstance.shader = Shader("Shaders/Diffuse.glsl");
	DiffuseMaterialInstance.albedo = Color::Grey();
	DiffuseMaterialInstance.shaderType = MaterialType::DiffuseMaterial;
	DiffuseMaterialInstance.shininess = 0.0f;
	DiffuseMaterialInstance.texture0 = DiffuseMaterialInstance.texture1 = DiffuseMaterialInstance.texture2 = DiffuseMaterialInstance.texture3 = 0;

	// Terrain splatmap
	TerrainSplatmapMaterialInstance.shader = Shader("Shaders/TerrainSplatmap.glsl");
	TerrainSplatmapMaterialInstance.albedo = Color::White();
	TerrainSplatmapMaterialInstance.shininess = 0.0f;
	TerrainSplatmapMaterialInstance.shaderType = MaterialType::TerrainSplatmapMaterial;
	TerrainSplatmapMaterialInstance.texture0 = Texture2D::MakeGLTexture("Data/Textures/grass.png", 0, true);
	TerrainSplatmapMaterialInstance.texture1 = Texture2D::MakeGLTexture("Data/Textures/sand.png", 1, true);
	TerrainSplatmapMaterialInstance.texture2 = Texture2D::MakeGLTexture("Data/Textures/rock.png", 3, true);
	TerrainSplatmapMaterialInstance.texture3 = Texture2D::MakeGLTexture("Data/Textures/snow.png", 4, true);

	// Simple Textured
	TexturedMaterialInstance.shader = Shader("Shaders/SimpleTextured.glsl");
	TexturedMaterialInstance.albedo = Color::Grey();
	TexturedMaterialInstance.shininess = 0.0f;
	TexturedMaterialInstance.shaderType = MaterialType::TexturedMaterial;
	TexturedMaterialInstance.texture0 = TerrainSplatmapMaterialInstance.texture0;
	TexturedMaterialInstance.texture1 = TexturedMaterialInstance.texture2 = TexturedMaterialInstance.texture3 = 0;

	// Wireframe
	WireframeMaterialInstance.shader = Shader("Shaders/Wireframe.glsl");
	WireframeMaterialInstance.albedo = Color::Green();
	WireframeMaterialInstance.shininess = 0.0f;
	WireframeMaterialInstance.shaderType = MaterialType::WireframeMaterial;
	WireframeMaterialInstance.texture0 = WireframeMaterialInstance.texture1 = WireframeMaterialInstance.texture2 = WireframeMaterialInstance.texture3 = 0;

	// Normal
	NormalMaterialInstance.shader = Shader("Shaders/Normal.glsl");
	NormalMaterialInstance.albedo = Color::Green();
	NormalMaterialInstance.shininess = 0.0f;
	NormalMaterialInstance.shaderType = MaterialType::NormalMaterial;
	NormalMaterialInstance.texture0 = WireframeMaterialInstance.texture1 = WireframeMaterialInstance.texture2 = WireframeMaterialInstance.texture3 = 0;
}

void Material::ReleaseStaticMaterials()
{
	DiffuseMaterialInstance.Release();
	TexturedMaterialInstance.Release();
	TerrainSplatmapMaterialInstance.Release();
	WireframeMaterialInstance.Release();
	NormalMaterialInstance.Release();
}
