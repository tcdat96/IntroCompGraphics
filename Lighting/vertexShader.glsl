#version 400

in vec3 vPosition;
in vec3 vNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(vPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * vNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}