#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
    vec3 position;
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outCutOff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(light.position - FragPos); //normalize(-light.direction);

	float theta = dot(lightDir, normalize(-light.direction));
	vec3 diffuse, specular, result;
	if(theta < light.outCutOff){
		result = ambient;
	}else {
		// diffuse
		vec3 norm = normalize(Normal);
		//vec3 lightDir = normalize(light.position - FragPos); //normalize(-light.direction);
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
		// here we inverse the sampled specular color. Black becomes white and white becomes black.
		// vec3 specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords)));

		result = diffuse + specular;

		if (theta < light.cutOff){
			// ����ھ۹�˥��������˥��
			float intensity = clamp((theta - light.outCutOff)/ (light.cutOff - light.outCutOff), 0.0, 1.0);
			result *= intensity;
		}

		// ���Ի��������۹�˥��
		result += ambient;
	}

	// add emission �����Է���
	result = result + vec3(texture(material.emission, TexCoords));

	// ���ݾ���˥��
	float dis = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dis + light.quadratic * dis * dis);	
	result = attenuation * result;
	FragColor = vec4(result, 1.0f);
}