#pragma once

#include "shader.h"
#include "transform.h"
#include "color.h"

enum MaterialType
{
	TerrainSplatmapMaterial = 1,
	DiffuseMaterial = 2,
	TexturedMaterial = 3,
	NormalMaterial = 4,
	WireframeMaterial = 5,
};

//
// Todo : Refactor this to make sure materials only store the right properties.
// -Diffuse should only have albedo and shininess
// -TerrainSplatmap should only have textures
// -SimpleTextured : one gl texture
// -Normal/Wireframe : nothing except for the overriden functions.
class Material
{
public:
	Shader shader;
	MaterialType shaderType;

	Color albedo;
	float shininess;

	GLuint texture0;
	GLuint texture1;
	GLuint texture2;
	GLuint texture3;

	Material();
	Material(const std::string& shaderPath, const Color& alb, double shin);

	virtual bool operator==(const Material& m);
	virtual void SetShader(const Shader& s);
	virtual void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos);
	virtual void SetFrameUniforms(const Transform& mvp, const Vector3 camPos);
	virtual void Release();

	/* Static Materials */
	static void InitStaticMaterials();
	static void ReleaseStaticMaterials();
	static Material TexturedMaterialInstance;
	static Material DiffuseMaterialInstance;
	static Material TerrainSplatmapMaterialInstance;
	static Material NormalMaterialInstance;
	static Material WireframeMaterialInstance;
};
