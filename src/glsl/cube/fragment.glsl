#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool enableOutline;
uniform vec4 outlineColor;

void main()
{
	if(!enableOutline) {
		FragColor = texture(texture1, TexCoord);
	}
	else {
		FragColor = outlineColor;
	}
};