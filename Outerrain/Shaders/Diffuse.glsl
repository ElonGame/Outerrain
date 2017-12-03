#version 330

#ifdef VERTEX_SHADER
	layout(location= 0) in vec3 position;
	uniform mat4 mvpMatrix;
	uniform mat4 mvMatrix;
	out vec3 vertex_position;
	
	void main( )
	{
		gl_Position = mvpMatrix * vec4(position, 1);
		vertex_position = vec3(mvMatrix * vec4(position, 1));
	}
#endif


#ifdef FRAGMENT_SHADER
	in vec3 vertex_position;
	out vec4 fragment_color;

	void main( )
	{
		fragment_color = vec4(1.0, 0.0, 0.0, 1.0);
	}
#endif
