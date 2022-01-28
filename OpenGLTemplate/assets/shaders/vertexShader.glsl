#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

out vec4 fColor;

void main() {
	fColor = aColor;
	gl_Position = uProjection * uView * (uTransform * vec4(aPos, 1.0));
}