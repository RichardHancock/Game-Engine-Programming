#version 330 core

layout(location = 0) in vec4 vPosition;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vTexCoord;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vPosition;

	vTexCoord = vec3(vPosition);
}
