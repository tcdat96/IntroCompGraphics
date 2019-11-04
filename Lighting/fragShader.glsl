#version 400

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
const vec3 lightColor = vec3(1,1,1);
uniform vec3 fColor;

void main() {
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
	float specular = 0;
	if (diff > 0) {
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	}
        
    vec3 result = (ambient + diffuse + specular * lightColor) * fColor;
    FragColor = vec4(result, 1.0);
}