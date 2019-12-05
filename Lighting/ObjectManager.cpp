#include "ObjectManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

glm::mat4 ObjectManager::sView = glm::lookAt(CAMERA, vec3(0), vec3(0, 1, 0));

ObjectManager::ObjectManager() {
	Solid::vPositions = {};

	generateObjects();

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 100000 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
}

void ObjectManager::generateObjects()
{
	PlanetSpec specs[] = {
		PlanetSpec("textures/sun.jpg", 8, 0, 0, 0, true),
		PlanetSpec("textures/mercury.jpg", 1, 11, 58.6f, 87.97f),
		PlanetSpec("textures/venus.jpg", 1.75, 17, 243, 224.7f),
		PlanetSpec("textures/earth.jpg", 2, 24, 0.99f, 365.26f),
		PlanetSpec("textures/mars.jpg", 1.5f, 32, 1.03f, 686.2f),
		PlanetSpec("textures/jupiter.jpg", 4, 40, 0.41f, 4328.9f),
		PlanetSpec("textures/saturn.jpg", 3, 48, 0.45f, 10752.9f),
		PlanetSpec("textures/uranus.jpg", 2.5, 56, 0.72f, 30663.65f)
		
	};
	for (auto spec : specs) {
		Solid* solid = createPlanet(spec);
		mObjects.push_back(solid);
	}

	PlanetSpec moonSpec = PlanetSpec("textures/moon.jpg", 0.5, 24, 27, 27.322f);
	mObjects.push_back(createSatellite(moonSpec, specs[3]));
}

Solid* ObjectManager::createPlanet(PlanetSpec spec) {
	auto indices = createSphere();
	int textureId = readTexture(spec.texture);
	Planet* planet = new Planet(indices, textureId, spec);
	planet->transform(spec.xfm);
	return planet;
}

Solid* ObjectManager::createSatellite(PlanetSpec satSpec, PlanetSpec center) {
	auto indices = createSphere();
	int textureId = readTexture(satSpec.texture);
	Satellite* satellite = new Satellite(indices, textureId, satSpec, center);
	satellite->transform(satSpec.xfm);
	return satellite;
}

std::vector<unsigned int> ObjectManager::createSphere() {
	int stacks = 10;
	int slices = stacks;
	float radius = 1;

	int offset = (Solid::vPositions.size()) / 3;

	for (int i = 0; i <= stacks; ++i) {

		float V = i / (float)stacks;
		float phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			float U = j / (float)slices;
			float theta = U * (glm::pi <float>() * 2);

			float x = cosf(theta) * sinf(phi);
			float y = cosf(phi);
			float z = sinf(theta) * sinf(phi);

			Solid::vPositions.push_back(x * radius);
			Solid::vPositions.push_back(y * radius);
			Solid::vPositions.push_back(z * radius);
		}
	}

	std::vector<unsigned int> indices;
	for (int i = 0; i < slices * stacks + slices; ++i) {
		indices.push_back(i);
		indices.push_back(i + slices + 1);
		indices.push_back(i + slices);

		indices.push_back(i + slices + 1);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	for (unsigned int i = 0; i < indices.size(); i++) {
		indices[i] += offset;
	}

	return indices;
}

int ObjectManager::readTexture(const char* textureFile) {
	int w;
	int h;
	int comp;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = stbi_load(textureFile, &w, &h, &comp, STBI_rgb_alpha);
	if (image == nullptr) {
		throw(std::string("Failed to load texture"));
	}
	
	int textureId = mObjects.size();
	glActiveTexture(GL_TEXTURE0 + textureId);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);

	return textureId++;
}

void ObjectManager::setLightPosition(float x, float y, float z) {
	mLight[0] = x; mLight[1] = y; mLight[2] = z;
	mLight[6] = x + 1; mLight[7] = y; mLight[8] = z - 1;
	mLight[12] = x - 1; mLight[13] = y; mLight[14] = z - 1;

	glm::vec3 v0 = vec3(mLight[0], mLight[1], mLight[2]);
	glm::vec3 v1 = vec3(mLight[6], mLight[7], mLight[8]);
	glm::vec3 v2 = vec3(mLight[12], mLight[13], mLight[14]);
	glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

	for (int i = 0; i < 3; i++) {
		mLight[3 + i * 6] = normal[0];
		mLight[4 + i * 6] = normal[1];
		mLight[5 + i * 6] = normal[2];
	}
}

void ObjectManager::render() {
	// debugging
	glUniformMatrix4fv(Solid::sViewLocation, 1, GL_FALSE, glm::value_ptr(sView));
	glUniformMatrix4fv(Solid::sModelLocation, 1, GL_FALSE, glm::value_ptr(mat4(1)));

	//for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
	//	(*it)->render(sView, mRotation);
	//}

	for (unsigned int i = 0; i < mObjects.size(); i++) {
		mObjects[i]->render(sView, mRotation);
	}
}
