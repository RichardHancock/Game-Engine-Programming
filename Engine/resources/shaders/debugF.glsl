#version 320 core

in vec3 fColour;

out vec4 fragColour;

void main()
{
	fragColour = vec4(fColour, 1.0f);
}
