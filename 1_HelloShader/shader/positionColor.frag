#version 330 core

in vec4 outPosition;
out vec4 frag_Color;
void main(){
	frag_Color = outPosition;
}