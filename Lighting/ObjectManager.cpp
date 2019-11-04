#include "ObjectManager.h"

glm::mat4 ObjectManager::sView = glm::lookAt(
	vec3(50, 10, 40),
	vec3(TRANSLATE_DELTA * 4, 0, 0),
	vec3(0, 1, 0)
);

ObjectManager::ObjectManager() {
	float phi = (1 + (float)sqrt(5)) / 2;
	float iphi = 1.0f / phi;
	Solid::vPositions = {
		// cube
		// tetrahedron 
		-1,-1,-1,
		1,-1,-1,
		-1,1,-1,
		1,1,-1,
		-1,-1,1,
		1,-1,1,
		-1,1,1,
		1,1,1,
		// octahedron - 8
		1,0,0,
		-1,0,0,
		0,1,0,
		0,-1,0,
		0,0,1,
		0,0,-1,
		// icosahedron - 14
		-1,0,phi,
		1,0,phi,
		-1,0,-phi,
		1,0,-phi,
		0,phi,1,
		0,phi,-1,
		0,-phi,1,
		0,-phi,-1,
		phi,1,0,
		-phi,1,0,
		phi,-1,0,
		-phi,-1,0,
		// dodecahedron - 26
		0,iphi,phi,
		0,-iphi,phi,
		0,iphi,-phi,
		0,-iphi,-phi,
		iphi,phi,0,
		-iphi,phi,0,
		iphi,-phi,0,
		-iphi,-phi,0,
		phi,0,iphi,
		-phi,0,iphi,
		phi,0,-iphi,
		-phi,0,-iphi,
		// MyHouse - 38
		-10,-4,-4,		// walls 38
		10,-4,-4,
		-10,4,-4,
		10,4,-4,
		-10,-4,4,
		10,-4,4,
		-10,4,4,
		10,4,4,
		-10,8,0,		// roof 46
		10,8,0,
		-7,-4,4,		// walls 48
		0,-4,4,
		-7,0,4,
		0,0,4,
		-7,-4,8,
		0,-4,8,
		-7,0,8,
		0,0,8,
		-3.5f,2,8,		// roof 56
		-3.5f,2,4,
		-5,-4,8,		// door 58
		-2,-4,8,
		-5,-1,8,
		-2,-1,8,
		-9,1,4,			// windows 62
		-7,1,4,
		-9,3,4,
		-7,3,4,
		-10,1,4,
		-7,4,4,
		3,1,4,
		8,1,4,
		3,3,4,
		8,3,4,
		8,4,4
	};

	//for (int i = 0; i < 38; i++) {
	//	Solid::vPositions[i] *= 3;
	//}

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 1000 * 2 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	generateObjects();
}

void ObjectManager::generateObjects()
{
	Solid* tetrahedron = new Solid(std::vector<unsigned int> {
		7, 2, 4,
			7, 4, 1,
			7, 1, 2,
			1, 4, 2,
	});
	Solid* octahedron = new Solid(std::vector<unsigned int> {
		8, 10, 12,
			8, 13, 10,
			8, 12, 11,
			8, 11, 13,
			9, 12, 10,
			9, 10, 13,
			9, 11, 12,
			9, 13, 11,
	});
	Solid* cube = new Solid(std::vector<unsigned int> {
		0, 2, 3, 1,
			4, 5, 7, 6,
			1, 3, 7, 5,
			0, 4, 6, 2,
			7, 3, 2, 6,
			5, 4, 0, 1
	}, 4);
	Solid* icosahedron = new Solid(std::vector<unsigned int> {
		15, 18, 14,
			18, 23, 14,
			18, 19, 23,
			22, 19, 18,
			15, 22, 18,
			15, 24, 22,
			24, 17, 22,
			22, 17, 19,
			17, 16, 19,
			17, 21, 16,
			17, 24, 21,
			24, 20, 21,
			20, 25, 21,
			20, 14, 25,
			20, 15, 14,
			24, 15, 20,
			25, 14, 23,
			16, 25, 23,
			19, 16, 23,
			25, 16, 21
	});
	Solid* dodecahedron = new Solid(std::vector<unsigned int> {
		7, 30, 31, 6, 26,
			2, 31, 30, 3, 28,
			3, 30, 7, 34, 36,
			6, 31, 2, 37, 35,
			0, 33, 4, 35, 37,
			5, 32, 1, 36, 34,
			29, 1, 32, 33, 0,
			27, 4, 33, 32, 5,
			28, 3, 36, 1, 29,
			29, 0, 37, 2, 28,
			26, 6, 35, 4, 27,
			27, 5, 34, 7, 26
	}, 5);
	Solid* myHouse = new MyHouse();

	mObjects.push_back(myHouse);
	mObjects.push_back(octahedron);
	mObjects.push_back(dodecahedron);
	mObjects.push_back(icosahedron);
	mObjects.push_back(tetrahedron);
	mObjects.push_back(cube);

	// initial translation
	for (unsigned int i = 1; i < mObjects.size(); i++) {
		mObjects[i]->setInitialTransX(TRANSLATE_DELTA * 2 + TRANSLATE_DELTA * i);
	}
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

	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 6; j++) {
	//		std::cout << mLight[i * 6 + j] << " ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "\n";
}

void ObjectManager::setViewIndex(unsigned int index) {
	if (index > 0 && index <= mObjects.size() + 1 && index != mViewIndex) {
		mViewIndex = index;
	}
}

void ObjectManager::render() {
	if (mViewIndex > 1) {
		mObjects[mViewIndex - 2]->render(mRotation);
	}
	else {
		// debugging
		//glUniform3f(Solid::sColorLocation, 1, 1, 1);
		glUniformMatrix4fv(Solid::sViewLocation, 1, GL_FALSE, glm::value_ptr(sView));
		glUniformMatrix4fv(Solid::sModelLocation, 1, GL_FALSE, glm::value_ptr(mat4(1)));
		glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof(GLfloat), mLight);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
			(*it)->render(sView, mRotation);
		}
	}
}
