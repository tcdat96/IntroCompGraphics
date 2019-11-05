#version 400

#define PI 3.14159265

out vec4 fragColor;

in vec3 fNormal;  
in vec3 fPosition;  
  
uniform vec3 lightPos1, lightPos2, lightPos3;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

const float F0 = 0.8;
const float roughness = 0.1;
const float k = 0.2;

vec3 calcLight(vec3 lightPosition) {
	float shiny = shininess;

    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPosition - fPosition);
    float NdotL = max(0, dot(norm, lightDir));
	vec3 viewDir = normalize(viewPos - fPosition);

	float Rs = 0.0;
	if (NdotL > 0) {
		vec3 H = normalize(lightDir + viewDir);
		float NdotH = max(0, dot(norm, H));
		float NdotV = max(0, dot(norm, viewDir));
		float VdotH = max(0, dot(lightDir, H));

		// Fresnel reflectance
		float F = pow(1.0 - VdotH, 5.0);
		F *= (1.0 - F0);
		F += F0;

		// Microfacet distribution by Beckmann
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (m_squared * NdotH * NdotH);
		float D = r1 * exp(r2);

		// Geometric shadowing
		float two_NdotH = 2.0 * NdotH;
		float g1 = (two_NdotH * NdotV) / VdotH;
		float g2 = (two_NdotH * NdotL) / VdotH;
		float G = min(1.0, min(g1, g2));

		Rs = (F * D * G) / (PI * NdotL * NdotV);
	}
	return ambientColor + diffuseColor * NdotL + specularColor * NdotL * (k + Rs * (1.0 - k));
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
}