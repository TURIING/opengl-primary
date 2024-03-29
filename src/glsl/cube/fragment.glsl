#version 460 core

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (std140, binding = 1) uniform Light {
	PointLight pointLight;
};

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// 定义材质属性
// 移除了环境光材质颜色向量，是因为环境光颜色在几乎所有情况下都等于漫反射颜色
struct Material {
	sampler2D diffuse;  						// 漫反射贴图
	sampler2D specular; 						// 镜面贴图
	float shininess;    						// 反光度，镜面高光的散射/半径
};

uniform Material material;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 outlineColor;
uniform bool enableOutline;
uniform bool enableTexture;

uniform vec3 cameraPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);

	if(!enableOutline) {
		FragColor = vec4(result, 1.0);
	}
	else {
		FragColor = outlineColor;
	}
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}