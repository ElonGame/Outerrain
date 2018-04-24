#pragma once

#include "shader.h"
#include "transform.h"
#include "color.h"

enum MaterialType
{
	TerrainSplatmapMaterial = 1,
	DiffuseMaterial = 2,
	SingleTexturedMaterial = 3,
	NormalMaterial = 4,
	WireframeMaterial = 5,
};

class MaterialBase
{
public:
	Shader shader;
	MaterialType shaderType;

	MaterialBase();
	MaterialBase(const std::string& shaderPath);

	virtual bool operator==(const MaterialBase& m);
	virtual void SetShader(const Shader& s);
	virtual void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3& camPos);
	virtual void SetFrameUniforms(const Transform& mvp, const Vector3& camPos);
	virtual void Release();

	/* Static Materials */
	static void InitStaticMaterials();
	static void ReleaseStaticMaterials();
	static MaterialBase* SingleTexturedMaterialInstance;
	static MaterialBase* DiffuseMaterialInstance;
	static MaterialBase* TerrainSplatmapMaterialInstance;
	static MaterialBase* NormalMaterialInstance;
	static MaterialBase* WireframeMaterialInstance;
};

class MaterialDiffuse : public MaterialBase
{
public:
	Color albedo;
	float shininess;

	MaterialDiffuse();
	MaterialDiffuse(const std::string& shaderPath);
	virtual void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3& camPos);
	virtual void SetFrameUniforms(const Transform& mvp, const Vector3& camPos);
};

class MaterialSingleTextured : public MaterialBase
{
public:
	GLuint texture0;

	MaterialSingleTextured();
	MaterialSingleTextured(const std::string& shaderPath);
	virtual void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3& camPos);
	virtual void SetFrameUniforms(const Transform& mvp, const Vector3& camPos);
	virtual void Release();
};

class MaterialTerrain : public MaterialBase
{
public:
	GLuint texture0;
	GLuint texture1;
	GLuint texture2;
	GLuint texture3;

	MaterialTerrain();
	MaterialTerrain(const std::string& shaderPath);
	virtual void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3& camPos);
	virtual void SetFrameUniforms(const Transform& mvp, const Vector3& camPos);
	virtual void Release();
};

class MaterialNormal : public MaterialBase
{
public:
	MaterialNormal();
	MaterialNormal(const std::string& shaderPath);
};

class MaterialWireframe : public MaterialBase
{
public:
	MaterialWireframe();
	MaterialWireframe(const std::string& shaderPath);
};