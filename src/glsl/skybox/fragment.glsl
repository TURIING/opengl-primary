#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

struct Material {
    samplerCube skybox;
};
uniform Material material;

void main()
{
    FragColor = texture(material.skybox, TexCoords);
}