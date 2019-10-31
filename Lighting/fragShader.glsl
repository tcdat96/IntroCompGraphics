#version 400
in vec3 color;
out vec4 fragColor;
void main() {
	if(gl_FrontFacing) {
		fragColor = vec4(color, 1.0);
	}
	else {
		fragColor = vec4(0.8, 0.8, 0.8, 1.0);
	}
}