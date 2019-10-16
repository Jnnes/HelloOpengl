#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D ourTexture;

void main()
{
    fragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);//vec4(TexCoord, 0.0f, 1.0f)
}