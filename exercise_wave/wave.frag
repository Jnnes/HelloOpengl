#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 FragPos;

#define MUTIL_CLICK_COUNT 1000
uniform sampler2D ourTexture;
uniform vec2 click[MUTIL_CLICK_COUNT];
uniform float clicktime[MUTIL_CLICK_COUNT];
uniform int clickCount;
uniform float curtime;

void main()
{	
	//FragColor = vec4(aFragColor, 1.0f);
	vec2 uv = TexCoords;
	float a;
	float tempDis;
	vec2 dir;

	for (int i = 0; i < clickCount; i++){
		// 计算方向及距离
		dir = FragPos - click[i];
		tempDis = sqrt(dir.x * dir.x +  dir.y * dir.y);

		float speed = 0.3;

		// 当前位置到，距离从点击位置传播一段时间后的距离
		float disOffset = clamp(((curtime + 1 - clicktime[i])* speed - tempDis) * 6, 0.0,6.283) - 3.1416; // 波传播到的位置与前位置的距离
		
		// 当前位置的振幅
		a = pow((cos(disOffset) + 1) / 2, 2) /80;
		
		// 振幅乘以 当前位置的高度（近似高度，通过偏移uv模拟）
		a *= cos((tempDis - (curtime + 1 - clicktime[i]) * speed) * 30);		
		
		uv += a *  normalize(dir);
	}

	if (false)
		FragColor = vec4(uv, 0.0, 1.0);
	else
		FragColor = vec4(vec3(texture(ourTexture, vec2(uv))), 1.0f);

}