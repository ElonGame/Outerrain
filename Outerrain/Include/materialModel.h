#pragma once

#include "shader.h"
#include "transform.h"
#include "color.h"

enum ShaderType
{
	TerrainSplatmap = 0,
	DiffuseGrey = 1,
	SimpleTextured = 2
};

class MaterialModel
{
public:
	Shader shader;
	ShaderType shaderType;

	Color albedo;
	float shininess;

	GLuint texture0;
	GLuint texture1;
	GLuint texture2;
	GLuint texture3;


	MaterialModel();
	virtual bool operator==(const MaterialModel& m);
	virtual void SetShader(const Shader& s);
	virtual void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos);
	virtual void Release();

	/* Static Materials */
	static void InitStaticMaterials();
	static void ReleaseStaticMaterials();
	static MaterialModel DefaultTexturedMat;
	static MaterialModel DefaultDiffuseMat;
	static MaterialModel TerrainTexturedMat;
};
