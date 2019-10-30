#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
};

struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	vec3 direction;

	float cutOff;
	float outCutOff;

	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight,vec3 normal, vec3 FragPos, vec3 viewDir);

void main()
{
	// 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light,vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));

	vec3 ambient, result;
	// ambient
	ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	if (theta < light.outCutOff){
		result = ambient;
	}else {
		vec3 diffuse, specular;
		// diffuse 
		float diff = max(dot(normal, lightDir), 0.0);
		diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		//specular
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
		specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
		
		if (theta < light.cutOff){
			float k = clamp((theta - light.outCutOff)/ (light.cutOff - light.outCutOff), 0.0, 1.0);
			result =k * (ambient + diffuse + specular);
		}else{
			result = (ambient + diffuse + specular);
		}

		// 衰减
		float dis  = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));
		result = attenuation * result;
	}

	return result;
}