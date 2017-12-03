#pragma once
#include <string>
#include <GL\glew.h>

struct Transform;
struct Vector2;
struct Vector3;
struct Vector4;

class Shader
{
private:
	GLuint program;

public:
	Shader() : program(0) { }

	void InitFromFile(const char *filename, const char *definitions = "");
	int Release();
	int Reload(const char *filename, const char *definitions = "");
	int GetCompileErrors(std::string& errors);
	int PrintCompileErrors();

	void UniformUInt(const char *uniform, const unsigned int& v);
	void UniformInt(const char *uniform, const int& v);
	void UniformFloat(const char *uniform, const float& v);

	void UniformVec2(const char *uniform, const Vector2& v);
	void UniformVec3(const char *uniform, const Vector3& v);
	void UniformVec4(const char *uniform, const Vector4& v);

	void UniformTransform(const char *uniform, const Transform& v);
	void UniformTexture(const char *uniform, const int unit, const GLuint texture, const GLuint sampler = 0);

	const GLuint& GetProgram() const { return program; }
};

