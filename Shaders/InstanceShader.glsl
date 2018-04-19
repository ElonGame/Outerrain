#version 330

#ifdef VERTEX_SHADER
uniform mat4 mvpMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 trsMatrix;

out vec2 vertex_texcoord;
out vec3 worldPos;
out vec3 worldNormal;

void main( )
{
	gl_Position 	=  mvpMatrix * vec4(position, 1);
	worldPos 		= (mvpMatrix * trsMatrix * vec4(position, 1)).xyz;
	worldNormal 	= (mvpMatrix * trsMatrix * vec4(normal  , 0)).xyz;
	vertex_texcoord = texcoord;
}
#endif


#ifdef FRAGMENT_SHADER
uniform int shaderType;
uniform vec3 camPos;
uniform float shininess;
uniform vec4 albedo;

uniform vec3 lightDir;
uniform vec4 lightColor;
uniform vec4 lightAmbientColor;
uniform float lightStrength;

in vec2 vertex_texcoord;
in vec3 worldPos;
in vec3 worldNormal;

out vec4 fragment_color;


void main()
{
	fragment_color = vec4(worldNormal, 1.0);
}
#endif
