#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "Solid.h"
#include "MyHouse.h"

constexpr unsigned int TRANSLATE_DELTA = 6;

class ObjectManager {
private:
	static glm::mat4 sVP;

	std::vector<GLfloat> vPositions;
	std::vector<GLfloat> vColors;
	std::vector<Solid*> mObjects;

	GLuint VBO;
	GLuint IBO;

	int mViewIndex = 1;
	bool mIsSolid = true;
	bool mRotation = false;

	ObjectManager();

	~ObjectManager() {
		for (auto obj : mObjects) {
			delete obj;
		}
	}
	
	void generateColors();

	void setVertexColor(int index, int r, int g, int b);

	void generateObjects();

	int getBufferSize();

public:
	int mVertexCount = 0;

	static ObjectManager* getInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

	GLuint getVbo() {
		return VBO;
	}

	int getColorPtr() {
		return getBufferSize();
	}

	void setViewIndex(int index);

	void setProjectionMatrix(glm::mat4 projection);

	void setMvpLocation(GLuint location);

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};