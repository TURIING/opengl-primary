#version 460 core

layout (std140, binding = 1) uniform PointLight
{
	vec3 pl_position;
	vec3 pl_ambient;
	vec3 pl_diffuse;
	vec3 pl_specular;
};

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
uniform sampler2D texture1;
uniform vec4 outlineColor;
uniform bool enableOutline;
uniform bool enableTexture;

uniform vec3 cameraPos;

void main()
{
	// ambient
	vec3 ambient = pl_ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pl_position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pl_diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pl_specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;

	if(!enableOutline) {
		if(enableTexture) {
			FragColor = texture(texture1, TexCoord);
		}
		else {
			FragColor = vec4(result, 1.0);
		}
	}
	else {
		FragColor = outlineColor;
	}
};