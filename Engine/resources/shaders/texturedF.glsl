#version 320 core

in vec2 vTexCoord;

uniform sampler2D diffuseMap;

out vec4 fragColour;

void main()
{
	vec2 flipTexCoord = vec2(vTexCoord.x, 1.0 - vTexCoord.y);

	vec3 diffuseColour = texture2D(diffuseMap, flipTexCoord).xyz;

	fragColour = vec4( diffuseColour, 1.0f);
}
