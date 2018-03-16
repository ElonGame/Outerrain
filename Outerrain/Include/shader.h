#pragma once
#include <string>
#include <GL/glew.h>

struct Color;
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
	Shader(const std::string& filename, const std::string& definitions = "");

	void Attach();
	void Detach();

	int Release();
	int Reload(const std::string&filename, const std::string&definitions = "");
	int GetCompileErrors(std::string& errors);
	int PrintCompileErrors();

	void UniformUInt(const std::string& uniform, unsigned int v);
	void UniformInt(const std::string& uniform, int v);
	void UniformFloat(const std::string& uniform, float v);
	void UniformColor(const std::string& uniform, const Color& c);
	void UniformVec2(const std::string& uniform, const Vector2& v);
	void UniformVec3(const std::string& uniform, const Vector3& v);
	void UniformVec4(const std::string& uniform, const Vector4& v);
	void UniformTransform(const std::string& uniform, const Transform& v);
	void UniformTexture(const std::string& uniform, int unit, GLuint texture, GLuint sampler = 0);

	GLuint GetProgram() const { return program; }
};
