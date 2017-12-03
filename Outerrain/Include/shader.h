#pragma once
#include <string>
#include <GL\glew.h>

class Shader
{
private:
	GLuint program;

public:
	Shader() : program(0) { }

	//! cree un shader program. a detruire avec release_program( ).
	//! charge un seul fichier, les shaders sont separes par \#ifdef VERTEX_SHADER / \#endif et \#ifdef FRAGMENT_SHADER / \#endif.
	//! renvoie l'identifiant openGL du program et le program est selectionne (cf glUseProgram()).
	//! \param definitions est une chaine de caracteres pouvant comporter plusieurs lignes "#define what value\n"\n.
	GLuint InitFromFile(const char *filename, const char *definitions = "");

	//! detruit les shaders et le program.
	int Release();

	//! recharge les sources et recompile un shader program.
	int Reload(const char *filename, const char *definitions = "");

	//! renvoie les erreurs de compilation.
	int GetCompileErrors(std::string& errors);

	//! affiche les erreurs de compilation.
	int PrintCompileErrors();


	const GLuint& GetProgram() const { return program; }
};