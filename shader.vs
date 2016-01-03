#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform float iTime;


void main()
{	

    gl_Position = projection * view * model * vec4(position, 1.0f);
	fNormal = vec3(vec4(normal,1.0) * model);

    TexCoords = texCoords;
}