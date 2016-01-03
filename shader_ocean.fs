#version 330 core

in vec2 TexCoords;
in vec4 ObjPos;
in vec3 fNormal;


out vec4 color;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;



uniform vec3 viewPos;
uniform float applyT;
uniform float applyL;


void main()
{    
	float ambStrength = 0.2f;
	float specStrength = 0.3f;

	vec3 Light1Color = vec3(0.246,0.0150,0.0);
	vec3 Light1Pos = vec3(1.0,1.0,2200.0);
	
	vec3 ambLight = vec3(1.0,1.0,1.0);
	vec4 ambFinal = vec4(ambLight * ambStrength,1.0) * texture(texture_diffuse1, TexCoords);
	
	vec3 norm = vec4(texture(texture_normal1,TexCoords)).xyz;

	vec3 Light1Dir = normalize(Light1Pos - ObjPos.xyz);
	float diff = max(dot(Light1Dir,norm),0);
	vec4 diffuse = vec4(diff * Light1Color,1.0f) * texture(texture_diffuse1, TexCoords);
	
	vec3 viewDir = normalize(viewPos - ObjPos.xyz);
	float spec = pow(max(dot(norm,viewDir),0.0),10);
	vec4 finalSpec = vec4(spec * specStrength * Light1Color,1.0) *  texture(texture_specular1, TexCoords);
	
	float nightFactor = 0.3f;

	if(applyT == 0.0)
	{
		color = vec4(0.0,0.0,1.0,1.0);
	}
	else 
	{
		if(applyL == 1.0)
		{
    		color = (diffuse + ambFinal)  + finalSpec;
    	}
    	else
    	{
    		color = texture(texture_diffuse1, TexCoords);
    	}
    }
    //color = vec4(texture(texture_diffuse1, TexCoords));
    color = vec4(color.rgb,0.7);
}