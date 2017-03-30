#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vColour;

uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 fColour;

void main()
{
	gl_Position = projMat * viewMat * vPosition;

	fColour = vColour;
}
