#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform float applyT;
uniform float applyL;

void main()
{   
	if(applyT == 0.0)
	{
		color = vec4(0.0,0.0,0.5,1.0);
	}
	else 
	{
    	color = vec4(texture(texture_diffuse1, TexCoords));
    }
}