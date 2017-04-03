#version 330 core
out vec4 FragColor;

//Parts taken from PGG2 and various OpenGL Tutorials

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointlight;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
float shininess = 32.0f;

vec3 CalcDirLight(DirLight light, vec3 normal, vec2 texCoords, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec2 texCoords, vec3 fragPos, vec3 viewDir);

void main()
{
	DirLight dirLight;
	dirLight.direction = vec3(-0.2f, -0.4f, -0.3f);
	dirLight.ambient = vec3(0.6f, 0.6f, 0.6f);
	dirLight.diffuse = vec3(0.2f, 0.2f, 0.2f);
	dirLight.specular = vec3(0.4f, 0.4f, 0.4f);

	vec2 flipTexCoords = vec2(fs_in.TexCoords.x, 1.0 - fs_in.TexCoords.y);

	vec4 diffuse = texture(diffuseMap, flipTexCoords);
	float alpha = diffuse.w;
	
    // Obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, flipTexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

	vec3 result = CalcDirLight(dirLight, normal, flipTexCoords, viewDir);
	result += CalcPointLight(pointlight, normal, flipTexCoords, fs_in.FragPos, viewDir);
	FragColor = vec4(result, alpha);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec2 texCoords, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularMap, texCoords));
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec2 texCoords, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularMap, texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
