#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 aFragColor;

uniform sampler2D ourTexture;

void main()
{	
	//FragColor = vec4(aFragColor, 1.0f);
	FragColor = vec4(vec3(texture(ourTexture, TexCoords)), 1.0f);
}