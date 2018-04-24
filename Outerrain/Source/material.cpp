#include "material.h"
#include "texture2D.h"
#include "mainwindow.h"


// Base Material
MaterialBase::MaterialBase()
{
}

MaterialBase::MaterialBase(const std::string& shaderPath)
{
	shader = Shader(shaderPath);
}

void MaterialBase::SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3& camPos)
{
	shader.Attach();

	shader.UniformVec3("lightDir", MainWindow::sceneLight.Direction());
	shader.UniformColor("lightColor", MainWindow::sceneLight.GetColor());
	shader.UniformColor("lightAmbientColor", MainWindow::sceneLight.AmbientColor());
	shader.UniformFloat("lightStrength", MainWindow::sceneLight.Strength());

	shader.UniformTransform("trsMatrix", trs);
	shader.UniformTransform("mvpMatrix", mvp);
	shader.UniformVec3("camPos", camPos);
	shader.UniformInt("shaderType", shaderType);
}

void MaterialBase::SetFrameUniforms(const Transform& mvp, const Vector3& camPos)
{
	shader.Attach();

	shader.UniformVec3("lightDir", MainWindow::sceneLight.Direction());
	shader.UniformColor("lightColor", MainWindow::sceneLight.GetColor());
	shader.UniformColor("lightAmbientColor", MainWindow::sceneLight.AmbientColor());
	shader.UniformFloat("lightStrength", MainWindow::sceneLight.Strength());

	shader.UniformTransform("mvpMatrix", mvp);
	shader.UniformVec3("camPos", camPos);
	shader.UniformInt("shaderType", shaderType);
}

void MaterialBase::Release()
{
	shader.Release();
}

bool MaterialBase::operator==(const MaterialBase& m)
{
	if (shaderType != m.shaderType)
		return false;
	return true;
}

void MaterialBase::SetShader(const Shader& s)
{
	shader = s;
}

// Static Materials
MaterialBase* MaterialBase::DiffuseMaterialInstance;
MaterialBase*  MaterialBase::TerrainSplatmapMaterialInstance;
MaterialBase*  MaterialBase::SingleTexturedMaterialInstance;
MaterialBase*  MaterialBase::WireframeMaterialInstance;
MaterialBase*  MaterialBase::NormalMaterialInstance;

void MaterialBase::InitStaticMaterials()
{
	// Diffuse
	MaterialDiffuse* diff = new MaterialDiffuse();
	diff->shader = Shader("Shaders/Diffuse.glsl");
	diff->albedo = Color::Grey();
	diff->shaderType = MaterialType::DiffuseMaterial;
	diff->shininess = 0.0f;
	MaterialBase::DiffuseMaterialInstance = diff;

	// Terrain splatmap
	MaterialTerrain* terrain = new MaterialTerrain();
	terrain->shader = Shader("Shaders/TerrainSplatmap.glsl");
	terrain->shaderType = MaterialType::TerrainSplatmapMaterial;
	terrain->texture0 = Texture2D::MakeGLTexture("Data/Textures/grass.png", 0, true);
	terrain->texture1 = Texture2D::MakeGLTexture("Data/Textures/sand.png", 1, true);
	terrain->texture2 = Texture2D::MakeGLTexture("Data/Textures/rock.png", 3, true);
	terrain->texture3 = Texture2D::MakeGLTexture("Data/Textures/snow.png", 4, true);
	MaterialBase::TerrainSplatmapMaterialInstance = terrain;

	// Simple Textured
	MaterialSingleTextured* singleTex = new MaterialSingleTextured();
	singleTex->shader = Shader("Shaders/SimpleTextured.glsl");
	singleTex->shaderType = MaterialType::SingleTexturedMaterial;
	singleTex->texture0 = Texture2D::MakeGLTexture("Data/Textures/grass.png", 0, true);
	MaterialBase::SingleTexturedMaterialInstance = singleTex;

	// Wireframe
	MaterialWireframe* wireframe = new MaterialWireframe();
	wireframe->shader = Shader("Shaders/Wireframe.glsl");
	wireframe->shaderType = MaterialType::WireframeMaterial;
	MaterialBase::WireframeMaterialInstance = wireframe;

	// Normal
	MaterialNormal* normal = new MaterialNormal();
	normal->shader = Shader("Shaders/Normal.glsl");
	normal->shaderType = MaterialType::NormalMaterial;
	MaterialBase::NormalMaterialInstance = normal;
}

void MaterialBase::ReleaseStaticMaterials()
{
	DiffuseMaterialInstance->Release();
	SingleTexturedMaterialInstance->Release();
	TerrainSplatmapMaterialInstance->Release();
	WireframeMaterialInstance->Release();
	NormalMaterialInstance->Release();
}


// Diffuse Material
MaterialDiffuse::MaterialDiffuse() : MaterialBase()
{
	shaderType = MaterialType::DiffuseMaterial;
}

MaterialDiffuse::MaterialDiffuse(const std::string& shaderPath) : MaterialBase(shaderPath)
{
	shaderType = MaterialType::DiffuseMaterial;
}

void MaterialDiffuse::SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3& camPos)
{
	MaterialBase::SetFrameUniforms(trs, mvp, camPos);
	shader.UniformColor("albedo", albedo);
	shader.UniformFloat("shininess", shininess);
}

void MaterialDiffuse::SetFrameUniforms(const Transform& mvp, const Vector3& camPos)
{
	MaterialBase::SetFrameUniforms(mvp, camPos);
	shader.UniformColor("albedo", albedo);
	shader.UniformFloat("shininess", shininess);
}


// Single Textured
MaterialSingleTextured::MaterialSingleTextured() : MaterialBase()
{
	shaderType = MaterialType::SingleTexturedMaterial;
}

MaterialSingleTextured::MaterialSingleTextured(const std::string & shaderPath) : MaterialBase(shaderPath)
{
	shaderType = MaterialType::SingleTexturedMaterial;
}

void MaterialSingleTextured::SetFrameUniforms(const Transform & trs, const Transform & mvp, const Vector3& camPos)
{
	MaterialBase::SetFrameUniforms(trs, mvp, camPos);
	shader.UniformInt("useTexture", 1);
	shader.UniformTexture("texture0", 0, texture0);
}

void MaterialSingleTextured::SetFrameUniforms(const Transform & mvp, const Vector3& camPos)
{
	MaterialBase::SetFrameUniforms(mvp, camPos);
	shader.UniformInt("useTexture", 1);
	shader.UniformTexture("texture0", 0, texture0);
}

void MaterialSingleTextured::Release()
{
	MaterialBase::Release();
	glDeleteTextures(1, &texture0);
}


// Terrain
MaterialTerrain::MaterialTerrain() : MaterialBase()
{
	shaderType = MaterialType::TerrainSplatmapMaterial;
}

MaterialTerrain::MaterialTerrain(const std::string & shaderPath) : MaterialBase(shaderPath)
{
	shaderType = MaterialType::TerrainSplatmapMaterial;
}

void MaterialTerrain::SetFrameUniforms(const Transform & trs, const Transform & mvp, const Vector3& camPos)
{
	MaterialBase::SetFrameUniforms(trs, mvp, camPos);
	shader.UniformInt("useTexture", 1);
	if (texture0 != 0)
		shader.UniformTexture("texture0", 0, texture0);
	if (texture1 != 0)
		shader.UniformTexture("texture1", 1, texture1);
	if (texture2 != 0)
		shader.UniformTexture("texture2", 2, texture2);
	if (texture3 != 0)
		shader.UniformTexture("texture3", 3, texture3);
}

void MaterialTerrain::SetFrameUniforms(const Transform & mvp, const Vector3& camPos)
{
	MaterialBase::SetFrameUniforms(mvp, camPos);
	shader.UniformInt("useTexture", 1);
	if (texture0 != 0)
		shader.UniformTexture("texture0", 0, texture0);
	if (texture1 != 0)
		shader.UniformTexture("texture1", 1, texture1);
	if (texture2 != 0)
		shader.UniformTexture("texture2", 2, texture2);
	if (texture3 != 0)
		shader.UniformTexture("texture3", 3, texture3);
}

void MaterialTerrain::Release()
{
	MaterialBase::Release();
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteTextures(1, &texture3);
}


// Normal
MaterialNormal::MaterialNormal() : MaterialBase()
{
	shaderType = MaterialType::NormalMaterial;
}

MaterialNormal::MaterialNormal(const std::string & shaderPath) : MaterialBase(shaderPath)
{
	shaderType = MaterialType::NormalMaterial;
}


// Wireframe
MaterialWireframe::MaterialWireframe() : MaterialBase()
{
	shaderType = MaterialType::WireframeMaterial;
}

MaterialWireframe::MaterialWireframe(const std::string & shaderPath) : MaterialBase(shaderPath)
{
	shaderType = MaterialType::WireframeMaterial;
}
