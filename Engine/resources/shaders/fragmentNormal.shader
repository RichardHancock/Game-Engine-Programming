#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

void main()
{           
	vec2 flipTexCoords = vec2(fs_in.TexCoords.x, 1.0 - fs_in.TexCoords.y);

    // Obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, flipTexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // Get diffuse color
    vec3 color = texture(diffuseMap, flipTexCoords).rgb;
    // Ambient
    vec3 ambient = 0.1 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.6f) * spec * texture(specularMap, flipTexCoords).rgb;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}