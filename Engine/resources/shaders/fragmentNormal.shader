#version 330 core
out vec4 FragColor;

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

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{           
	DirLight dirLight;
	dirLight.direction = vec3(-0.5f, -0.6f, -0.4f);
	dirLight.ambient = vec3(0.6f, 0.6f, 0.6f);
	dirLight.diffuse = vec3(0.5f, 0.5f, 0.5f);
	dirLight.specular = vec3(0.4f, 0.4f, 0.4f);

	vec2 flipTexCoords = vec2(fs_in.TexCoords.x, 1.0 - fs_in.TexCoords.y);

    // Obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, flipTexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	
	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	FragColor = vec4(result, 1.0f);
	
    // Get diffuse color
    //vec3 color = texture(diffuseMap, flipTexCoords).rgb;
    // Ambient
    //vec3 ambient = 0.1 * color;
    // Diffuse
    //vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    //float diff = max(dot(lightDir, normal), 0.0);
    //vec3 diffuse = diff * color;
    // Specular
    //vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    //vec3 reflectDir = reflect(-lightDir, normal);
    //vec3 halfwayDir = normalize(lightDir + viewDir);  
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    //vec3 specular = vec3(0.6f) * spec * texture(specularMap, flipTexCoords).rgb;
    
    //FragColor = vec4(ambient + diffuse + specular, 1.0f);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec2 flipTexCoords = vec2(fs_in.TexCoords.x, 1.0 - fs_in.TexCoords.y);

    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, flipTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, flipTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularMap, flipTexCoords));
    return (ambient + diffuse + specular);
}

