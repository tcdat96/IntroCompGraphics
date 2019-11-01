#version 400

in vec3 fPosition;
in vec3 fNormal;

uniform vec3 lightPos;
uniform vec3 fColor;

out vec4 fragColor;

const vec3 lightColor = vec3(1,1,1);
const float ambient = 0.2;

void main() {
	vec3 objectColor = fColor;
	if (!gl_FrontFacing) {
		objectColor = vec3(0.8, 0.8, 0.8);
	}
  	
    // diffuse 
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fPosition);
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient * objectColor + diffuse) * objectColor;
	fragColor = vec4(result, 1.0);
}