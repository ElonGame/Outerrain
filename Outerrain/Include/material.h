#pragma once

#include "shader.h"
#include "transform.h"
#include "color.h"

enum ShaderType
{
	TerrainSplatmap = 0,
	DiffuseGrey = 1,
	SimpleTextured = 2,
	Wireframe = 3,
};

class Material
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
	static Material DefaultTexturedMat;
	static Material DefaultDiffuseMat;
	static Material TerrainTexturedMat;
	static Material WireframeMat;
};
