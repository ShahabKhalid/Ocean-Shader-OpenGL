#version 330 core

in vec2 TexCoords;
in vec3 fNormal;

out vec4 color;


uniform sampler2D texture_diffuse1;
uniform float applyT;
uniform float applyL;


uniform float iTime;

struct DirectLight
{
	vec3 Color;
	vec3 Dir;
};

void main()
{    
	DirectLight DLight1;
	DLight1.Color = vec3(1.0,0.3,0.0);
	DLight1.Dir = vec3(-1.0,0.5,1.0);

	

	vec3 norm = normalize(fNormal);

	float dirL = max(dot(DLight1.Dir,norm),0.0);
	vec4 dirLightC = dirL * vec4(DLight1.Color,1.0) * 0.1f;
	
	float nightFactor = 0.2f;

	if(applyT == 0.0)
	{
		color = vec4(0.3,0.2,0.1,1.0);
	}
	else 
	{
		if(applyL == 0.0)
		{
    		color = vec4(texture(texture_diffuse1, TexCoords));
    	}
    	else
    	{
    		
    		color = vec4(texture(texture_diffuse1, TexCoords)) * nightFactor + dirLightC;
    		color = vec4(color.rgb,1.0); // alpha ko full kya wapis
    	}
    }
    //color = vec4(texture(texture_normal1, TexCoords)//);
}