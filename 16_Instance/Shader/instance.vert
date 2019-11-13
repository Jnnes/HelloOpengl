#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 fragColor;

uniform vec2 translate;

void main(){
	gl_Position = vec4(aPos + translate, 0.0, 1.0);
	fragColor = aColor;
}
