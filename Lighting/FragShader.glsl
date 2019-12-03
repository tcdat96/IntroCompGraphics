#version 400

out vec4 fragColor;

in vec3 fNormal;  
in vec3 fPosition;  
  
uniform vec3 lightPos1, lightPos2, lightPos3;
uniform vec3 viewPos;

const float ambientColor = 0.1;
const float shininess = 32;

uniform sampler2D surfaceTexture;

vec3 calcLight(vec3 normal, vec3 lightPosition) {
	// diffuse 
    vec3 lightDir = normalize(lightPosition - fPosition);
    float lambertian = max(dot(normal, lightDir), 0.0);
    
    // specular
	float specular = 0;
	if (lambertian > 0) {
		vec3 viewDir = normalize(viewPos - fPosition);
		vec3 reflectDir = reflect(-lightDir, normal);  
		specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}

	return ambientColor + lambertian + specular;
}

void main() {
    vec3 normal = normalize(fNormal);

    vec3 color = vec3(0);
	color += calcLight(normal, lightPos1);
	if (lightPos2 != vec3(0)) {
		color += calcLight(normal, lightPos2);
	}
	if (lightPos3 != vec3(0)) {
		color += calcLight(normal, lightPos3);
	}
	fragColor = vec4(color, 1.0);

	vec2 longitudeLatitude = vec2((atan(normal.z, normal.x) / 3.1415926 - 1.0) * 0.5,
                                  (asin(normal.y) / 3.1415926 + 0.5));
    vec4 texColor = texture(surfaceTexture, longitudeLatitude);
	fragColor = fragColor * texColor;
}