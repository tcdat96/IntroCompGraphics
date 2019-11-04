#version 400

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

void main() { 	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float lambertian = max(dot(norm, lightDir), 0.0);
    
    // specular
	float specular = 0;
	if (lambertian > 0) {
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
        
    FragColor = vec4(ambientColor + lambertian * diffuseColor + specular * specularColor, 1.0);
}