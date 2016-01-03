#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec4 ObjPos;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float iTime;



void main()
{	
	vec3 pos = position;	
	highp int poszi = int(pos.z);
	highp int posxi = int(pos.x);
	pos.y = pos.y + sin(iTime * (pos.z - poszi) * ((pos.x - posxi) * 3f) * 2f) * 4.0f;



	vec3 norm = normal;
	//norm.z = sin(pos.y / 10.0f) / 10.0f;

    gl_Position = projection * view * model * vec4(pos, 1.0f);

    ObjPos = model * vec4(pos,1.0f);
	fNormal = vec3(vec4(norm,1.0) * model);

    TexCoords = vec2(texCoords.x,texCoords.y + sin(iTime * 0.5f) / 5.0);
}