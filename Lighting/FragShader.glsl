#version 400

out vec4 fragColor;

in vec3 fNormal;  
in vec3 fPosition;  
  
uniform vec3 lightPos1, lightPos2, lightPos3;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform float shininess;

uniform sampler2D surfaceTexture;

vec3 calcLight(vec3 lightPosition) {
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

	return ambientColor + lambertian + specular;
}

void main() {
    vec3 color = vec3(0);
	color += calcLight(lightPos1);
	if (lightPos2 != vec3(0)) {
		color += calcLight(lightPos2);
	}
	if (lightPos3 != vec3(0)) {
		color += calcLight(lightPos3);
	}
	fragColor = vec4(color, 1.0);

    vec3 norm = normalize(fNormal);
	vec2 longitudeLatitude = vec2((atan(norm.z, norm.x) / 3.1415926 - 1.0) * 0.5,
                                  (asin(norm.y) / 3.1415926 + 0.5));
    vec4 texColor = texture(surfaceTexture, longitudeLatitude);
	fragColor = fragColor * texColor;
}