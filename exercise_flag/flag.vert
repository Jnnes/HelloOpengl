#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float col; // ��
uniform float row; // ��
uniform float count; // ���в����

void main()
{
	FragPos = aPos;
	float det = 1.0 / count;
	TexCoords = vec2(row * det,1.0 - (col + 1) * det) + aTexCoords / count;
	
	vec3 pos = aPos;
	pos.z = 10.0 * row /count * sin(time + 3.0 * 3.1416 * (count - row) / count);

	gl_Position = projection * view * model * vec4(pos, 1.0);
};