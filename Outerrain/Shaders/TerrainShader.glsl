
#version 330

#ifdef VERTEX_SHADER
uniform mat4 mvpMatrix;
uniform mat4 trsMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

out vec2 vertex_texcoord;
out vec3 worldPos;
out vec3 worldNormal;

void main( )
{
	gl_Position = mvpMatrix * vec4(position, 1);
	vertex_texcoord = texcoord;
	worldPos = (trsMatrix * vec4(position, 1)).xyz;
	worldNormal = (trsMatrix * vec4(normal, 0)).xyz;
}
#endif


#ifdef FRAGMENT_SHADER
uniform int renderMode;
uniform vec3 camPos;
uniform vec4 diffuseColor;
uniform float shininess;

uniform sampler2D texture0;

const vec3 ambientLight = vec3(0.1, 0.1, 0.1);
const vec3 lightDir = vec3(0.707, -0.707, 0);
const vec3 lightColor = vec3(1, 1, 1);
const float lightStrength = 1.0;

in vec2 vertex_texcoord;
in vec3 worldPos;
in vec3 worldNormal;

out vec4 fragment_color;


vec3 NormalShading()
{
	return worldNormal;
}

vec3 TerrainShading(vec2 uv)
{
	// Terrain material placement
	vec3 grassColor = vec3(0.3, 0.6, 0.15);
	vec3 snowColor = vec3(1, 1, 1);
	vec3 rockColor = vec3(0.4, 0.4, 0.4);
	vec3 dirtColor = vec3(0.4, 0.25, 0.2);
	
	// Grass/Rock
	float slope = 1.0 - worldNormal.y;
	vec3 terrainColor = mix(grassColor, dirtColor, clamp(pow(slope * 5.0, 2), 0, 1));
	terrainColor = mix(terrainColor, rockColor, pow(slope, 4));
	
	// Snow
	float illumination = texture(texture0, uv).r;
	terrainColor = mix(terrainColor, snowColor, clamp(pow(1.0 - illumination, 2) * 3.0, 0, 1));
	
	// Diffuse term (Lambert)
	float diffuse = max(0.0, dot(-lightDir, worldNormal));

	// Specular term (Blinn Phong)
	float specular = 0;
	if(diffuse > 0 && shininess > 0)
	{
		vec3 viewDir = normalize(camPos - worldPos);
		vec3 halfDir = normalize(-lightDir + viewDir);
		float specAngle = max(dot(halfDir, worldNormal), 0.0);
		specular = pow(specAngle, shininess);
	}

	// Final color
	return ambientLight 
			+ diffuse * terrainColor.rgb * (lightColor * lightStrength) 
			+ specular * (lightColor * lightStrength);
}

void main()
{
	if (renderMode == 1) 	  // Normal
		fragment_color = vec4(NormalShading(), 1.0);
	else if (renderMode == 2) // WireFrame
		fragment_color = vec4(0.0, 0.8, 0.3, 1.0);
	else if (renderMode == 0) // Diffuse Splatmap
		fragment_color = vec4(TerrainShading(vertex_texcoord.xy), 1.0);
	else
		fragment_color = vec4(texture(texture0, vertex_texcoord.xy).rgb, 1);
	//fragment_color = vec4(vertex_texcoord.xy, 0, 1);
}
#endif
