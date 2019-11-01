#version 400

in vec3 vPosition;
in vec3 vNormal;

uniform mat4 modelView;
uniform mat4 projection;

out vec3 fPosition;
out vec3 fNormal;

void main() {
	gl_Position = projection * modelView * vec4(vPosition, 1.0);

	vec4 vertPos4 = modelView * vec4(vPosition, 1.0);
    fPosition = vec3(vertPos4) / vertPos4.w;

	fNormal = vNormal;
}