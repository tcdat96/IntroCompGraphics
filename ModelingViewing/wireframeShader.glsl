#version 400
in vec3 vPosition;
uniform vec3 wColor;
uniform mat4 MVP;
out vec3 color;
void main() {
	gl_Position = MVP * vec4(vPosition, 1.0);
	color = wColor;
}