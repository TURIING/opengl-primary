#version 460 core

#define MAX_LIGHT_NUM 10
#define LIGHT_TYPE_POINTLIGHT 2
#define LIGHT_TYPE_BLINNPHONG 3
#define MATERIAL_TYPE_PHONG 1
#define MATERIAL_TYPE_REFLECT 2
#define MATERIAL_TYPE_COMMON 3

struct PointLight {
	vec3 position;
	float constant;

	vec3 ambient;
	float linear;

	vec3 diffuse;
	float quadratic;

	vec3 specular;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (std140, binding = 1) uniform Light {
	PointLight pointLight[MAX_LIGHT_NUM];
	DirectionalLight directionalLight[MAX_LIGHT_NUM];
	int actualPointLightNum;
	int actualDirectionalLightNum;
};

layout (std140, binding = 2) uniform CameraInfo {
	vec3 cameraPos;
};

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

// 定义材质属性
// 移除了环境光材质颜色向量，是因为环境光颜色在几乎所有情况下都等于漫反射颜色
struct Material {
	int type;									// 材质类型

	// PhongMaterial
	sampler2D diffuse;  						// 漫反射贴图
	sampler2D specular; 						// 镜面贴图
	float shininess;    						// 反光度，镜面高光的散射/半径

	// ReflectMaterial
	samplerCube skybox;
};

uniform Material material;
uniform vec4 outlineColor;
uniform bool enableOutline;
uniform bool enableTexture;
uniform bool enableShadow;
uniform sampler2D shadowMap;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	if(material.type == MATERIAL_TYPE_PHONG || material.type == MATERIAL_TYPE_COMMON) {
		vec3 result = vec3(0, 0, 0);
		for(int i = 0; i < actualDirectionalLightNum; i++) {
			result += CalcDirLight(directionalLight[i], norm, viewDir);
		}

		for(int i = 0; i < actualPointLightNum; i++) {
			result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
		}

		FragColor = vec4(result, 1.0);
	}
	else if(material.type == MATERIAL_TYPE_REFLECT) {
		vec3 reflectDir = reflect(-viewDir, norm);
		FragColor = vec4(texture(material.skybox, reflectDir).rgb, 1.0);
	}
	else {
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 halfWayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfWayDir), 0.0), material.shininess);
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

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//	vec3 halfWayDir = normalize(lightDir + viewDir);
//	float spec = pow(max(dot(normal, halfWayDir), 0.0), material.shininess);

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	if(enableShadow) {
		float shadow = ShadowCalculation(FragPosLightSpace);
		return ambient + (1.0 - shadow) * (diffuse + specular);
	}
	else {
		return (ambient + diffuse + specular);
	}
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// Calculate bias (based on depth map resolution and slope)
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight[0].direction - FragPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	// Check whether current frag pos is in shadow
	// float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if(projCoords.z > 1.0)
	shadow = 0.0;

	return shadow;
}