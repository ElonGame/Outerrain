#version 330

/* Lights Information */
struct LightSource
{
	int type;
	float range;
	vec3 position;
	vec3 direction;
	vec3 color;
};
const int nbLights = 1;
uniform LightSource lights[1];


#ifdef VERTEX_SHADER
uniform mat4 u_mvpMatrix;

layout(location = 0) in vec3 i_vertex;
layout(location = 1) in vec2 i_texcoord;
layout(location = 2) in vec3 i_normal;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec2 v_texcoord;

void main( )
{
	gl_Position = u_mvpMatrix * vec4(i_vertex, 1);
	v_vertex = i_vertex;
	v_normal = i_normal;
	v_texcoord = i_texcoord;
}
#endif


#ifdef FRAGMENT_SHADER
uniform bool u_use_texture;
uniform sampler2D u_texture;
uniform vec3 u_camPos;
uniform vec4 u_diffuse;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec2 v_texcoord;

layout(location = 0) out vec4 fragment_color;
layout(location = 1) out vec3 fragment_normal;
layout(location = 2) out vec3 fragment_position;

void main()
{
	vec3 viewDir = normalize(u_camPos - v_vertex);
	vec3 normal = normalize(v_normal);
	vec3 finalColor = vec3(0.0);
	for (int i = 0; i < nbLights; i++)
	{
		vec3 normalizedLightDir = normalize(lights[i].direction);
		vec3 halfDir = normalize(lights[i].direction + viewDir);
		
		vec3 objectColor = (u_diffuse.xyz / 255.0f);
		if (u_use_texture)
			objectColor *= texture(u_texture, v_texcoord).xyz;
		
		float diff = max(dot(normalizedLightDir, normal), 0.0);
		vec3 diffuse = diff * lights[i].color;
		
		finalColor += diffuse * objectColor;
	}
	fragment_position = v_vertex;
	fragment_normal = normal * 0.5 + 0.5;
	fragment_color = vec4(finalColor, 1.0);
}
#endif