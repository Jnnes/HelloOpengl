#version 330 core
out vec4 FragColor;

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
	float constant;
	float linear;
	float quadratic;

	vec3 direction;
	float cutOff;
	float outCutOff;	
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform int dirLightNum;
#define NR_POINT_LIGHTS 16
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int pointLightNum;
#define NR_SPOT_LIGHTS 16
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int spotLightNum;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight,vec3 normal, vec3 FragPos, vec3 viewDir);

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{
	// 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result ;

    // 第一阶段：定向光照,暂定定向光源一个
	if (dirLightNum > 0)
		result += CalcDirLight(dirLight, norm, viewDir);

    // 第二阶段：点光源
    for(int i = 0; i < pointLightNum; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
		
    // 第三阶段：聚光，暂定聚光一个
	for(int i = 0; i< spotLightNum; i++)
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);   	

    FragColor = vec4(result, 1.0);

	float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
	FragColor = vec4(vec3(depth), 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
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
	ambient = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
	if (theta < light.outCutOff){
		result = ambient;
	}else {
		vec3 diffuse, specular;
		// diffuse 
		float diff = max(dot(normal, lightDir), 0.0);
		diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

		//specular
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
		specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
		
		if (theta < light.cutOff){
			float k = clamp((theta - light.outCutOff)/ (light.cutOff - light.outCutOff), 0.0, 1.0);
			result =k * (ambient + diffuse + specular);
		}else{
			result = (ambient + diffuse + specular);
		}

		// 衰减
		float dis  = length(light.position - fragPos);
		float attenuation = 1.0 / ( light.constant + light.linear * dis + light.quadratic * (dis * dis));
		result = attenuation * result;
	}

	return result;
}