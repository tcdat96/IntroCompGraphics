#version 400

in vec3 vPosition;
in vec3 vNormal;

out vec3 fPosition;
out vec3 fNormal;
out vec3 fUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fPosition = vec3(model * vec4(vPosition, 1.0));
    fNormal = mat3(transpose(inverse(model))) * vNormal;
	fUV = vNormal;
    
    gl_Position = projection * view * vec4(fPosition, 1.0);
}