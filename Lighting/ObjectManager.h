#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "Satellite.h"

constexpr glm::vec3 CAMERA = vec3(70, 25, 70);

class ObjectManager {
private:
	static glm::mat4 sView;

	std::vector<Solid*> mObjects;
	std::vector<unsigned int> mSphereIndices;

	std::vector<vec3> mParticles;

	GLuint mVBO;
	GLfloat mLight[18];

	bool mRotation = false;

	ObjectManager();

	~ObjectManager() {
		for (auto obj : mObjects) {
			delete obj;
		}
	}

	void generateObjects();
	Solid* createPlanet(PlanetSpec spec);
	Solid* createSatellite(PlanetSpec satellite, PlanetSpec center);
	void initSphere();
	std::vector<unsigned int> getSphere();

public:
	int mVertexCount = 0;

	static ObjectManager* getInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

	void setUniformLocations(GLint model, GLint view, GLint texture, GLint lightSource) {
		Solid::sModelLocation = model;
		Solid::sViewLocation = view;
		Solid::sTextureLocation = texture;
		Solid::sLightSourceLocation = lightSource;
	}

	int readTexture(const char* textureFile);

	void setLightPosition(float x, float y, float z);

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};