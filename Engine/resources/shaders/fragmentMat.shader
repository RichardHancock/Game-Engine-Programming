#version 430 core

in vec3 lightDirV;
in vec3 vNormalV;
in vec2 vTexCoord;

uniform vec3 lightColour = {1,1,1};
uniform vec3 emissiveColour = {0.5f,0,0};
uniform vec3 ambientColour = {0.1f,0.3f,0.1f};
uniform vec3 specularColour = {0.6f,0.6f,0.6f};

uniform float shininess = 70.0f;
uniform float alpha = 1.0f;

uniform sampler2D gSampler;

out vec4 fragColour;

void main()
{
	vec2 flipTexCoord = vec2(vTexCoord.x, 1.0 - vTexCoord.y);
	
	vec3 diffuseColour = texture2D(gSampler, flipTexCoord).xyz;
	
	vec3 lightDir = normalize( lightDirV );
	vec3 vNormal = normalize( vNormalV );
	
	vec3 diffuse = diffuseColour * lightColour * max( dot( vNormal, lightDir ), 0);
	fragColour = vec4( emissiveColour + ambientColour + diffuse, alpha);
	
}