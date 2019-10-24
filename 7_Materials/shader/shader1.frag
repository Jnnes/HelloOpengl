#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diffStrength = 1.0f;
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diffStrength * diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(-FragPos); // 因为当前使用观察矩阵计算光照，所以，viewPos始终是原点
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}