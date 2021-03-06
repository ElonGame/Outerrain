#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <set>

#include "vec.h"
#include "color.h"
#include "shader.h"
#include "transform.h"

#ifdef _WIN32
#define SSCANF sscanf_s
#define SPRINTF sprintf_s
#else
#define SSCANF sscanf
#define SPRINTF sprintf
#endif


/* File scope */
static std::string read(const std::string& filename)
{
	std::stringbuf source;
	std::ifstream in(filename);
	if (in.good() == false)
		printf("[error] loading program '%s'...\n", filename.c_str());
	in.get(source, 0);
	return source.str();
}

static std::string prepare_source(std::string file, const std::string& definitions)
{
	if (file.empty())
		return std::string();

	// un peu de gymnastique, #version doit rester sur la premiere ligne, meme si on insere des #define dans le source
	std::string source;

	// recupere la ligne #version
	std::string version;
	size_t b = file.find("#version");
	if (b != std::string::npos)
	{
		size_t e = file.find('\n', b);
		if (e != std::string::npos)
		{
			version = file.substr(0, e + 1);
			file.erase(0, e + 1);
			if (file.find("#version") != std::string::npos)
			{
				printf("[error] found several #version directives. failed.\n");
				return std::string();
			}
		}
	}
	else
	{
		printf("[error] no #version directive found. failed.\n");
		return std::string();
	}

	// reconstruit le source complet
	if (definitions.empty() == false)
	{
		source.append(version);                         // insere la version
		source.append(definitions).append("\n");        // insere les definitions
		source.append(file);                            // insere le source
	}
	else
	{
		source.append(version);                         // re-insere la version (supprimee de file)
		source.assign(file);                            // insere le source
	}

	return source;
}

static void print_line(std::string& errors, const std::string&source, const int begin_id, const int line_id)
{
	int line = 0;
	char last = '\n';
	for (unsigned int i = 0; source[i] != 0; i++)
	{
		if (line > line_id)
			break;

		if (last == '\n')
		{
			line++;
			if (line >= begin_id && line <= line_id)
			{
				errors.append("  ");
				errors.push_back('0' + (line / 1000) % 10);
				errors.push_back('0' + (line / 100) % 10);
				errors.push_back('0' + (line / 10) % 10);
				errors.push_back('0' + (line / 1) % 10);
				errors.append("  ");
			}
		}

		if (line >= begin_id && line <= line_id)
		{
			if (source[i] == '\t')
				errors.append("    ");
			else
				errors.push_back(source[i]);
		}
		last = source[i];
	}
}

static int print_errors(std::string& errors, const std::string&log, const std::string&source)
{
	printf("[error log]\n%s\n", log.c_str());

	int first_error = INT_MAX;
	int last_string = -1;
	int last_line = -1;
	for (int i = 0; log[i] != 0; i++)
	{
		// recupere la ligne assiciee a l'erreur
		int string_id = 0, line_id = 0, position = 0;
		if (SSCANF(&log[i], "%d ( %d ) : %n", &string_id, &line_id, &position) == 2        // nvidia syntax
			|| SSCANF(&log[i], "%d : %d (%*d) : %n", &string_id, &line_id, &position) == 2  // mesa syntax
			|| SSCANF(&log[i], "ERROR : %d : %d : %n", &string_id, &line_id, &position) == 2  // ati syntax
			|| SSCANF(&log[i], "WARNING : %d : %d : %n", &string_id, &line_id, &position) == 2)  // ati syntax
		{
			if (string_id != last_string || line_id != last_line)
			{
				// conserve la premiere erreur
				first_error = std::min(first_error, line_id);

				// extrait la ligne du source...
				errors.append("\n");
				print_line(errors, source, last_line + 1, line_id);
				errors.append("\n");
			}
		}
		// et affiche l'erreur associee...
		for (i += position; log[i] != 0; i++)
		{
			errors.push_back(log[i]);
			if (log[i] == '\n')
				break;
		}

		last_string = string_id;
		last_line = line_id;
	}
	errors.append("\n");
	print_line(errors, source, last_line + 1, 1000);
	errors.append("\n");

	return first_error;
}

static std::string shader_string(const GLenum type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER: return std::string("vertex shader");
	case GL_FRAGMENT_SHADER: return std::string("fragment shader");
	case GL_GEOMETRY_SHADER: return std::string("geometry shader");
#ifdef GL_VERSION_4_0
	case GL_TESS_CONTROL_SHADER: return std::string("control shader");
	case GL_TESS_EVALUATION_SHADER: return std::string("evaluation shader");
#endif
#ifdef GL_VERSION_4_3
	case GL_COMPUTE_SHADER: return std::string("compute shader");
#endif
	default: return std::string("shader");
	}
}

static const char *shader_keys[] = { "VERTEX_SHADER", "FRAGMENT_SHADER", "GEOMETRY_SHADER", "TESSELATION_CONTROL", "EVALUATION_CONTROL", "COMPUTE_SHADER" };
const int shader_keys_max = 6;

static GLenum shader_types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER,
#ifdef GL_VERSION_4_0
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
#else
	0,
	0,
#endif
#ifdef GL_VERSION_4_3
	GL_COMPUTE_SHADER
#else
	0
#endif
};

static GLuint compile_shader(const GLuint program, const GLenum shader_type, const std::string& source)
{
	if (source.size() == 0 || shader_type == 0)
		return 0;

	GLuint shader = glCreateShader(shader_type);
	glAttachShader(program, shader);

	const char *sources = source.c_str();
	glShaderSource(shader, 1, &sources, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	return (status == GL_TRUE) ? shader : 0;
}


/* Shader Specifics */
Shader::Shader(const std::string& filename, const std::string& definitions)
{
	program = glCreateProgram();
	Reload(filename, definitions);
	PrintCompileErrors();
}

void Shader::Attach()
{
	glUseProgram(program);
}

void Shader::Detach()
{
	glUseProgram(0);
}

int Shader::Reload(const std::string&filename, const std::string& definitions)
{
	if (program == 0)
		return -1;

	int shaders_max = 0;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaders_max);
	if (shaders_max > 0)
	{
		std::vector<GLuint> shaders(shaders_max, 0);
		glGetAttachedShaders(program, shaders_max, NULL, &shaders.front());
		for (int i = 0; i < shaders_max; i++)
		{
			glDetachShader(program, shaders[i]);
			glDeleteShader(shaders[i]);
		}
	}

#ifdef GL_VERSION_4_3
	glObjectLabel(GL_PROGRAM, program, -1, filename.c_str());
#endif

	std::string common_source = read(filename);
	for (int i = 0; i < shader_keys_max; i++)
	{
		if (common_source.find(shader_keys[i]) != std::string::npos)
		{
			std::string source = prepare_source(common_source, std::string(definitions).append("#define ").append(shader_keys[i]).append("\n"));
			GLuint shader = compile_shader(program, shader_types[i], source);
			if (shader == 0)
				printf("[error] compiling %s...\n%s\n", shader_string(shader_types[i]).c_str(), definitions.c_str());
		}
	}

	// linke les shaders
	glLinkProgram(program);

	// verifie les erreurs
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		printf("[error] linking program %u '%s'...\n", program, filename.c_str());
		return -1;
	}

	PrintCompileErrors();

	// pour etre coherent avec les autres fonctions de creation, active l'objet gl qui vient d'etre cree.
	glUseProgram(program);
	return 0;
}

int Shader::Release()
{
	if (program == 0)
		return -1;

	int shaders_max = 0;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaders_max);
	if (shaders_max > 0)
	{
		std::vector<GLuint> shaders(shaders_max, 0);
		glGetAttachedShaders(program, shaders_max, NULL, &shaders.front());
		for (int i = 0; i < shaders_max; i++)
		{
			glDetachShader(program, shaders[i]);
			glDeleteShader(shaders[i]);
		}
	}
	glDeleteProgram(program);
	return 0;
}

int Shader::GetCompileErrors(std::string& errors)
{
	errors.clear();

	if (program == 0)
	{
		errors.append("[error] no program...\n");
		return -1;
	}

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		return 0;

	int first_error = INT_MAX;
	int shaders_max = 0;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaders_max);
	if (shaders_max == 0)
	{
		errors.append("[error] no shaders...\n");
		return 0;
	}

	std::vector<GLuint> shaders(shaders_max, 0);
	glGetAttachedShaders(program, shaders_max, NULL, &shaders.front());
	for (int i = 0; i < shaders_max; i++)
	{
		GLint value;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &value);
		if (value == GL_FALSE)
		{
			// recupere les erreurs de compilation des shaders
			glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &value);
			std::vector<char>log(value + 1, 0);
			glGetShaderInfoLog(shaders[i], (GLsizei)log.size(), NULL, &log.front());

			// recupere le source
			glGetShaderiv(shaders[i], GL_SHADER_SOURCE_LENGTH, &value);
			std::vector<char> source(value + 1, 0);
			glGetShaderSource(shaders[i], (GLsizei)source.size(), NULL, &source.front());

			glGetShaderiv(shaders[i], GL_SHADER_TYPE, &value);
			errors.append("[error] compiling ").append(shader_string(value)).append("...\n");

			// formatte les erreurs
			int last_error = print_errors(errors, &log.front(), &source.front());
			first_error = std::min(first_error, last_error);
		}
	}

	// recupere les erreurs de link du program
	{
		GLint value = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &value);

		std::vector<char>log(value + 1, 0);
		glGetProgramInfoLog(program, (GLsizei)log.size(), NULL, &log.front());

		errors.append("[error] linking program...\n").append(log.begin(), log.end());
	}

	return first_error;
}

int Shader::PrintCompileErrors()
{
	std::string errors;
	int code = GetCompileErrors(errors);
	if (errors.size() > 0)
		printf("%s\n", errors.c_str());
	return code;
}


static int location(const GLuint program, const std::string&uniform)
{
	if (program == 0)
		return -1;

	// recuperer l'identifiant de l'uniform dans le program
	GLint location = glGetUniformLocation(program, uniform.c_str());
	if (location < 0)
	{
		char error[1024] = { 0 };
#ifdef GL_VERSION_4_3
		{
			char label[1024];
			glGetObjectLabel(GL_PROGRAM, program, sizeof(label), NULL, label);

			//SPRINTF(error, "uniform( %s %u, '%s' ): not found.", label, program, uniform.c_str());
		}
#else
		//sprintf(error, "uniform( program %u, '%s'): not found.", program, uniform);
#endif
		static std::set<std::string> log;
		if (log.insert(error).second == true)
			printf("%s\n", error);
		return -1;
	}

#ifndef GK_RELEASE
	// verifier que le program est bien en cours d'utilisation, ou utiliser glProgramUniform, mais c'est gl 4
	GLuint current;
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&current);
	if (current != program)
	{
		char error[1024] = { 0 };
#ifdef GL_VERSION_4_3
		{
			char label[1024];
			glGetObjectLabel(GL_PROGRAM, program, sizeof(label), NULL, label);
			char labelc[1024];
			glGetObjectLabel(GL_PROGRAM, current, sizeof(labelc), NULL, labelc);

			SPRINTF(error, "uniform( %s %u, '%s' ): invalid shader program %s %u", label, program, uniform.c_str(), labelc, current);
		}
#else
		sprintf(error, "uniform( program %u, '%s'): invalid shader program %u...", program, uniform, current);
#endif

		printf("%s\n", error);
		glUseProgram(program);
	}
#endif

	return location;
}

void Shader::UniformUInt(const std::string&uniform, unsigned int v)
{
	glUniform1ui(location(program, uniform), v);
}

void Shader::UniformInt(const std::string&uniform, int v)
{
	glUniform1i(location(program, uniform), v);
}

void Shader::UniformFloat(const std::string&uniform, float v)
{
	glUniform1f(location(program, uniform), v);
}

void Shader::UniformVec2(const std::string&uniform, const Vector2& v)
{
	glUniform2fv(location(program, uniform), 1, (float*)&v.x);
}

void Shader::UniformVec3(const std::string&uniform, const Vector3& a)
{
	glUniform3fv(location(program, uniform), 1, (float*)&a.x);
}

void Shader::UniformVec4(const std::string&uniform, const Vector4& v)
{
	glUniform4fv(location(program, uniform), 1, (float*)&v.x);
}

void Shader::UniformColor(const std::string& uniform, const Color& c)
{
	glUniform4fv(location(program, uniform), 1, (float*)&c.r);
}

void Shader::UniformTransform(const std::string&uniform, const Transform& v)
{
	glUniformMatrix4fv(location(program, uniform), 1, GL_TRUE, (const float*)v.Buffer());
}

void Shader::UniformTexture(const std::string&uniform, int unit, GLuint texture, GLuint sampler)
{
	int id = location(program, uniform);
	if (id < 0)
		return;
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindSampler(unit, sampler);
	glUniform1i(id, unit);
}
