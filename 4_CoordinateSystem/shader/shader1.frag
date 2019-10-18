#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float angle;
uniform float opcity;

void main()
{
//    fragColor = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f);//vec4(TexCoord, 0.0f, 1.0f)
	float a = angle * 3.1415926 / 180;
	vec2 TexCoord2 = vec2(cos(a) * TexCoord.x - sin(a)*TexCoord.y, TexCoord.x * sin(a) + TexCoord.y * cos(a));
	//fragColor = texture(ourTexture1, TexCoord);
	
    fragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord2), opcity);
}