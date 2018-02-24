#pragma once

#include "shader.h"
#include "transform.h"
#include "color.h"

class MaterialModel
{
private:
	Shader shader;

	Color albedo;
	float shininess;

	GLuint texture0;
	GLuint texture1;
	GLuint texture2;
	GLuint texture3;

public:
	MaterialModel();

	void SetFrameUniforms(const Transform& trs, const Transform& mvp, const Vector3 camPos);
	void SetShader(const Shader& s);

	/* Static Materials */
	static MaterialModel DefaultDiffuse();
	static MaterialModel TerrainTextured();
};
