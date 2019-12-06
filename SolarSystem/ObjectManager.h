#pragma once

#define CAMERA_RADIUS 100

#include <vector>
#include <set>

#include "utils.h"
#include "Satellite.h"

class ObjectManager {
private:
	static glm::mat4 sView;

	std::vector<Solid*> mObjects;
	std::vector<unsigned int> mSphereIndices;

	std::vector<vec3> mParticles;

	float mCameraAngle = radians(45.0f);

	GLuint mVBO;
	GLfloat mLight[18];

	bool mRotation = false;

	ObjectManager();

	~ObjectManager() {
		for (auto obj : mObjects) {
			delete obj;
		}
	}

	static void updateViewMatrix() {
		sView = glm::lookAt(sCamera, vec3(0), vec3(0, 1, 0));
	}

	void generateObjects();
	Solid* createPlanet(PlanetSpec spec);
	Solid* createSatellite(PlanetSpec satellite, PlanetSpec center);
	void initSphere();
	std::vector<unsigned int> getSphere();
	int readTexture(const char* textureFile);

public:
	static glm::vec3 sCamera;
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

	void moveCameraVert(bool up = true);
	void moveCameraHoriz(bool left = true);

	void updateRotationSpeed(bool faster);

	void setLightPosition(float x, float y, float z);

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};