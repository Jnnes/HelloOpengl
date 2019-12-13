#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float colCount;// �е���Ŀ, X�ᱻ�ֶ���Ŀ
uniform float rowCount;// �е���Ŀ��Y�ᱻ�ֶ���Ŀ

void main()
{		
	float Y = floor(gl_InstanceID / colCount); 
	float X = floor(gl_InstanceID - Y * colCount);

	FragPos = aPos;
	float XDet = 1.0 / colCount;
	float YDet = 1.0 / rowCount;

	// ������������
	TexCoords = vec2(X * XDet,1.0 - (Y + 1) * YDet) + vec2(aTexCoords.x / colCount,aTexCoords.y / rowCount);
	//TexCoords = aTexCoords;

	vec3 pos = aPos;		
	pos.x += -colCount / 2 + X + 0.5;
	pos.y += -rowCount / 2 + Y + 0.5;

	// Խ�������Ƶ��Խ�󣬲���Խ��
	float ZDet = 1.0 / max(XDet, YDet) * X /colCount;
	float XN = 1.7;
	float YN = 0.4;
	
	pos.z += 0.3 * ZDet  * sin(time + XN * 6.283 * ((colCount - X) * (colCount - X) / colCount /colCount) + 0.2);
	pos.z += 0.2 * ZDet * sin(time + YN * 6.283 * (Y / rowCount + 0.2) );

	// �����ҷ���Ŷ�

	gl_Position = projection * view * model * vec4(pos, 1.0);
};