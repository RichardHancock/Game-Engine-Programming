#version 330 core

in vec3 vTexCoord;

uniform samplerCube diffuseMap;

out vec4 fragColour;

void main()
{
	fragColour = texture(diffuseMap, vTexCoord);
}
