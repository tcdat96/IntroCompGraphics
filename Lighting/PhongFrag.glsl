#version 400

out vec4 fragColor;

in vec3 fNormal;  
in vec3 fPosition;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

void main() { 	
    // diffuse 
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fPosition);
    float lambertian = max(dot(norm, lightDir), 0.0);
    
    // specular
	float specular = 0;
	if (lambertian > 0) {
		vec3 viewDir = normalize(viewPos - fPosition);
		vec3 reflectDir = reflect(-lightDir, norm);  
		specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
        
    fragColor = vec4(ambientColor + lambertian * diffuseColor + specular * specularColor, 1.0);
}