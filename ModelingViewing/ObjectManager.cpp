#include "ObjectManager.h"

glm::mat4 ObjectManager::sVP = glm::mat4(1);

ObjectManager::ObjectManager() {
	float phi = (1 + sqrt(5)) / 2;
	float iphi = 1.0 / phi;
	vPositions = {
		// cube
		// tetrahedron 
		-1,-1,-1,
		-1,-1,1,
		-1,1,-1,
		-1,1,1,
		1,-1,-1,
		1,-1,1,
		1,1,-1,
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
		-4,-4,-10,		// 38
		-4,-4,10,
		-4,4,-10,
		-4,4,10,
		4,-4,-10,
		4,-4,10,
		4,4,-10,
		4,4,10,
		0,8,-10,		// 46
		0,8,10,
		4,-4,-7,		// 48
		4,-4,0,
		4,0,-7,
		4,0,0,
		8,-4,-7,
		8,-4,0,
		8,0,-7,
		8,0,0,
		8,2,-3.5f,		// 56
		4,2,-3.5f,
		8,-4,-5,		// door 58
		8,-4,-2,
		8,-1,-5,
		8,-1,-2,
		4,1,-9,			// windows - 62
		4,1,-7,
		4,3,-9,
		4,3,-7,
		4,1,-10,
		4,4,-7,
		4,1,3,
		4,1,8,
		4,3,3,
		4,3,8,
		4,4,8
	};

	generateColors();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	int bufferSize = getBufferSize();
	glBufferData(GL_ARRAY_BUFFER, bufferSize * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, &vPositions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, bufferSize, bufferSize, &vColors[0]);

	generateObjects();
}

void ObjectManager::generateColors()
{
	vColors.clear();
	vColors.resize(vPositions.size());

	for (int i = 0; i <= 7; i++) {
		vColors[i * 3] = rand(0.25, 1);
	}

	for (int i = 8; i <= 13; i++) {
		vColors[i * 3 + 1] = rand(0.25, 1);
	}

	for (int i = 14; i <= 25; i++) {
		vColors[i * 3 + 2] = rand(0.25, 1);
	}

	for (int i = 26; i <= 37; i++) {
		vColors[i * 3] = rand(0.25, 1);
	}

	// house
	for (int i = 38 * 3; i < vPositions.size(); i++) {
		vColors[i] = 0.75;
	}

	setVertexColor(46, 1, 0, 0);
	setVertexColor(47, 1, 0, 0);
	setVertexColor(56, 1, 0, 0);
	setVertexColor(57, 1, 0, 0);

	for (int i = 58; i <= 72; i++) {
		setVertexColor(i, 0, 1, 0);
	}
}

void ObjectManager::setVertexColor(int index, int r, int g, int b)
{
	int offset = index * 3;
	vColors[offset] = r;
	vColors[offset + 1] = g;
	vColors[offset + 2] = b;
}

void ObjectManager::generateObjects()
{
	Solid* tetrahedron = new Solid(std::vector<unsigned int> {
		7, 4, 2,
			7, 4, 1,
			7, 1, 2,
			1, 2, 4,
	});
	Solid* octahedron = new Solid(std::vector<unsigned int> {
		8, 10, 12,
			8, 10, 13,
			8, 11, 12,
			8, 11, 13,
			9, 10, 12,
			9, 10, 13,
			9, 11, 12,
			9, 11, 13,
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
		14, 18, 15,
			14, 23, 18,
			23, 19, 18,
			18, 19, 22,
			18, 22, 15,
			22, 24, 15,
			22, 17, 24,
			19, 17, 22,
			19, 16, 17,
			16, 21, 17,
			21, 24, 17,
			21, 20, 24,
			21, 25, 20,
			25, 14, 20,
			14, 15, 20,
			20, 15, 24,
			23, 14, 25,
			23, 25, 16,
			23, 16, 19,
			21, 16, 25
	});
	Solid* dodecahedron = new Solid(std::vector<unsigned int> {
		26, 3, 31, 30, 7,
			28, 6, 30, 31, 2,
			36, 34, 7, 30, 6,
			35, 37, 2, 31, 3,
			37, 35, 1, 33, 0,
			34, 36, 4, 32, 5,
			29, 4, 32, 33, 0,
			27, 1, 33, 32, 5,
			29, 4, 36, 6, 28,
			29, 0, 37, 2, 28,
			27, 1, 35, 3, 26,
			27, 5, 34, 7, 26,
	}, 5);

	Solid* myHouse = new MyHouse();

	mObjects.push_back(myHouse);
	mObjects.push_back(octahedron);
	mObjects.push_back(dodecahedron);
	mObjects.push_back(icosahedron);
	mObjects.push_back(tetrahedron);
	mObjects.push_back(cube);

	// initial translation
	for (int i = 1; i < mObjects.size(); i++) {
		mObjects[i]->setInitialTransX(TRANSLATE_DELTA * 2 + TRANSLATE_DELTA * i);
	}
}


int ObjectManager::getBufferSize() {
	//return vPositions.size() * sizeof(glm::vec3);
	return vPositions.size() * sizeof(GLfloat);
}

void ObjectManager::setViewIndex(int index) {
	mViewIndex = index > 0 && index <= mObjects.size() + 1 ? index : mViewIndex;
}

void ObjectManager::setProjectionMatrix(glm::mat4 projection) {
	auto view = glm::lookAt(
		vec3(50, 8, 40),
		vec3(TRANSLATE_DELTA * 4, 0, 0),
		vec3(0, 1, 0)
	);
	sVP = projection * view;
	for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
		(*it)->setProjectionMatrix(projection);
	}
}

void ObjectManager::setMvpLocation(GLuint location) {
	for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
		(*it)->setMvpLocation(location);
	}
}

void ObjectManager::render() {
	if (mViewIndex > 1) {
		mObjects[mViewIndex - 2]->render(mRotation);
	}
	else {
		for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
			(*it)->render(sVP, mRotation);
		}
	}
}
