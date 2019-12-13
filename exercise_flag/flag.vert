#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float colCount;// 列的数目, X轴被分段数目
uniform float rowCount;// 行的数目，Y轴被分段数目

void main()
{		
	float Y = floor(gl_InstanceID / colCount); 
	float X = floor(gl_InstanceID - Y * colCount);

	FragPos = aPos;
	float XDet = 1.0 / colCount;
	float YDet = 1.0 / rowCount;

	// 计算纹理坐标
	TexCoords = vec2(X * XDet,1.0 - (Y + 1) * YDet) + vec2(aTexCoords.x / colCount,aTexCoords.y / rowCount);
	//TexCoords = aTexCoords;

	vec3 pos = aPos;
	pos.x += -colCount / 2 + X + 0.5;
	pos.y += -rowCount / 2 + Y + 0.5;

	// 越靠近旗杆频率越大，波长越短
	float ZDet = 1.0 / max(XDet, YDet) * X /colCount;
	float XN = 1.7;
	float YN = 0.5;
	
	// 增加乱风的扰动，从XY两个方向考虑，通过调整两个方向的波长与强度模拟
	pos.z += 0.3 * ZDet  * sin(3 * time + XN * 6.283 * ((colCount - X) * (colCount - X) / colCount /colCount) + 0.2);
	pos.z += 0.2 * ZDet * sin(2 * time + YN * 6.283 * (Y / rowCount + 0.2) );


	gl_Position = projection * view * model * vec4(pos, 1.0);
};