#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 FragPos;

void main()
{	
	TexCoords = aTexCoords;
	FragPos = aPos.xy;
	gl_Position =vec4(aPos, 1.0); // projection * view * model *  // 只用于显示波纹，省略3D坐标转换
};