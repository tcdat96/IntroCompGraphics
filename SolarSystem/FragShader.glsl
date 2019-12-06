#version 400

out vec4 fragColor;

in vec3 fNormal;  
in vec3 fPosition;
in vec3 fUV;

uniform bool lightSource = false;
uniform vec3 viewPos;

const vec3 lightPos = vec3(0);
const float ambientColor = 0.25;
const float shininess = 32;

uniform sampler2D surfaceTexture;

vec3 calcLight(vec3 lightPosition) {
	vec3 normal = normalize(fNormal);

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

	return vec3(ambientColor + lambertian + specular);
}

void main() {
	fragColor = vec4(1);
	if (!lightSource) {
		fragColor = vec4(calcLight(lightPos), 1.0);
	}

	vec3 normal = normalize(fUV);
	vec2 longitudeLatitude = vec2((atan(normal.z, normal.x) / 3.1415926 - 1.0) * 0.5,
                                  (asin(normal.y) / 3.1415926 + 0.5));
    fragColor *= texture(surfaceTexture, longitudeLatitude);
}