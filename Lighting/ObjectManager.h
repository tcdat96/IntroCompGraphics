#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "MyHouse.h"

constexpr float TRANSLATE_DELTA = 6;


class ObjectManager {
private:
	static glm::mat4 sVP;

	std::vector<Solid*> mObjects;

	int mViewIndex = 1;
	bool mIsSolid = true;
	bool mRotation = false;

	ObjectManager();

	~ObjectManager() {
		for (auto obj : mObjects) {
			delete obj;
		}
	}

	void generateObjects();

public:
	int mVertexCount = 0;

	static ObjectManager* getInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

	void setViewIndex(unsigned int index);

	void setProjectionMatrix(glm::mat4 projection);

	void setColorLocation(GLuint location) {
		Solid::sColorLocation = location;
	}

	void setMvpLocation(GLuint location) {
		Solid::sMvpLocation = location;
	}

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};