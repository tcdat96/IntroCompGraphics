#version 400

in vec3 vPosition;
in vec3 vNormal;

out vec4 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos1, lightPos2, lightPos3;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

vec3 calcLight(vec3 fPosition, vec3 fNormal, vec3 lightPosition) {
	// diffuse 
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPosition - fPosition);
    float lambertian = max(dot(norm, lightDir), 0.0);
    
    // specular
	float specular = 0;
	if (lambertian > 0) {
		vec3 viewDir = normalize(viewPos - fPosition);
		vec3 reflectDir = reflect(-lightDir, norm);  
		specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
        
    return ambientColor + lambertian * diffuseColor + specular * specularColor;
}

void main()
{
	vec3 fPosition = vec3(model * vec4(vPosition, 1.0));
    vec3 fNormal = mat3(transpose(inverse(model))) * vNormal;  
    gl_Position = projection * view * vec4(fPosition, 1.0);

	vec3 color = vec3(0);
	color += calcLight(fPosition, fNormal, lightPos1);
	if (lightPos2 != vec3(0)) {
		color += calcLight(fPosition, fNormal, lightPos2);
	}
	if (lightPos3 != vec3(0)) {
		color += calcLight(fPosition, fNormal, lightPos3);
	}
	fColor = vec4(color, 1.0);
}