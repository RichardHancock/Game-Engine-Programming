#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = projMat * viewMat * modelMat * position;
    vs_out.FragPos = vec3(modelMat * position);
    vs_out.TexCoords = texCoords;

    mat3 normalMat = transpose(inverse(mat3(modelMat)));
    vec3 T = normalize(normalMat * tangent);
    vec3 N = normalize(normalMat * normal);

	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
}
