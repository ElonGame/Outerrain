
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GL\glew.h"
#include "image.h"


//! \addtogroup openGL
///@{

//! \file 
//! texture2D openGL.

//! cree une texture a partir d'une image im. a detruire avec glDeleteTextures( ). 
//! \param texel_type permet de choisir la representation interne des valeurs de la texture.
GLuint make_texture( const int unit, const Image& im, const GLenum texel_type= GL_RGBA32F );

//! cree une texture a partir des donnees d'une image, cf image_io.h. a detruire avec glDeleteTextures( ).
//! \param texel_type permet de choisir la representation interne des valeurs de la texture.
GLuint make_texture( const int unit, const ImageData& im, const GLenum texel_type= GL_RGBA );

//! cree une texture a partir d'un fichier filename. a detruire avec glDeleteTextures( ).
//! \param texel_type permet de choisir la representation interne des valeurs de la texture.
GLuint read_texture( const int unit, const char *filename, const GLenum texel_type= GL_RGBA );

//! renvoie le nombre de mipmap d'une image width x height.
int miplevels( const int width, const int height );

///@}
#endif
