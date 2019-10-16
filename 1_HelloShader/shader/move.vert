#version 330 core
layout (location = 0) in vec3 position;
uniform vec2 moveOffset;

out vec4 outPosition;
void main(){
	gl_Position = vec4(position.x + moveOffset.x, position.y + moveOffset.y, position.z, 1.0f);
	outPosition = gl_Position;
}