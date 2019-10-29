#version 400
in vec3 vPosition;
uniform vec3 vColor;
uniform mat4 objectToWorld;
out vec3 color;
void main() {
	gl_Position = objectToWorld * vec4(vPosition, 1.0);
	color = vColor;
}