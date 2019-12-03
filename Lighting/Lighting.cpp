#include "Lighting.h"

GLuint VAO = 0;
GLuint gVertexProgram, gWireframeProgram;

ObjectManager* gObjectManager;

float gFovy = 45;
GLint gProjectionLocation;

int gLightCount = 1;
bool gSunEffect = false;
GLint gLight1Location, gLight2Location, gLight3Location;

GLint gTextureLocation;

int main(void)
{
	if (setUpOpenGlComponents() == -1) {
		getchar();
		return -1;
	}
	setUpData();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	do {
		renderWorld();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();

	return 0;
}

int setUpOpenGlComponents() {
	std::srand((unsigned int)std::time(nullptr));

	// GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Buffer Transformation", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// interaction
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glewExperimental = GL_TRUE;
	// GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	return 1;
}

void setUpData() {
	gObjectManager = ObjectManager::getInstance();
	setUpShaders();
	setUpProjection();
	setUpLight();
	setUpTexture();
}

void setUpProjection() {
	auto projection = glm::perspective(glm::radians(gFovy), 4.0f / 3.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(gProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void setUpLight() {
	static float sSunAngle = 0;
	sSunAngle += 0.005f;
	if (sSunAngle > M_PI * 2) sSunAngle = 0;

	auto radius = 50.0f;
	auto cosine = radius * cosf(sSunAngle);
	auto sine = radius * sinf(sSunAngle);

	auto lightPos = glm::vec3(cosine, 0, sine);
	glUniform3f(gLight1Location, lightPos[0], lightPos[1], lightPos[2]);
	gObjectManager->setLightPosition(lightPos[0], lightPos[1], lightPos[2]);

	if (gLightCount >= 2) {
		auto lightPos2 = glm::vec3(40, -cosine, -sine);
		glUniform3f(gLight2Location, lightPos2[0], lightPos2[1], lightPos2[2]);
	}

	if (gLightCount >= 3) {
		auto lightPos3 = glm::vec3(cosine, sine, -30);
		glUniform3f(gLight3Location, lightPos3[0], lightPos3[1], lightPos3[2]);
	}
}

void setUpTexture() {
	int w;
	int h;
	int comp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load("textures\\earth.jpg", &w, &h, &comp, STBI_rgb_alpha);

	if (image == nullptr)
		throw(std::string("Failed to load texture"));

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);
}

void setUpShaders() {
	// vertex shader
	gVertexProgram = initShaders(SHADER_VERTEX, SHADER_FRAG);
	glUseProgram(gVertexProgram);

	// position attribute
	GLint vPosition = glGetAttribLocation(gVertexProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

	// position attribute
	GLint vNormal = glGetAttribLocation(gVertexProgram, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	gProjectionLocation = getUniformLocation(gVertexProgram, "projection");
	gLight1Location = getUniformLocation(gVertexProgram, "lightPos1");
	gLight2Location = getUniformLocation(gVertexProgram, "lightPos2");
	gLight3Location = getUniformLocation(gVertexProgram, "lightPos3");

	auto viewPosLocation = getUniformLocation(gVertexProgram, "viewPos");
	glUniform3f(viewPosLocation, CAMERA[0], CAMERA[1], CAMERA[2]);

	gTextureLocation = getUniformLocation(gVertexProgram, "surfaceTexture");

	gObjectManager->setUniformLocations(
		getUniformLocation(gVertexProgram, "model"),
		getUniformLocation(gVertexProgram, "view"),
		getUniformLocation(gVertexProgram, "ambientColor"),
		getUniformLocation(gVertexProgram, "shininess")
	);
}

void renderWorld() { 
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gSunEffect) {
		setUpLight();
	}

	gObjectManager->render();
}

// input handler
void getCursorPos(double& x, double& y) {
	glfwGetCursorPos(window, &x, &y);
	x = (x * 2 - SCREEN_WIDTH) / SCREEN_WIDTH;
	y = -(y * 2 - SCREEN_HEIGHT) / SCREEN_HEIGHT;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_S:
			glUseProgram(gVertexProgram);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GLFW_KEY_W:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLFW_KEY_R:
			gObjectManager->toggleRotation();
			break;
		case GLFW_KEY_U:
			gSunEffect = !gSunEffect;
			break;
		case GLFW_KEY_L:
			gLightCount = gLightCount < 3 ? gLightCount + 1 : 1;
			glUniform3f(gLight2Location, 0, 0, 0);
			glUniform3f(gLight3Location, 0, 0, 0);
			break;
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
		case GLFW_KEY_5:
		case GLFW_KEY_6:
		case GLFW_KEY_7:
		case GLFW_KEY_8:
			gObjectManager->setViewIndex(key - GLFW_KEY_0);
			break;
		}
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	gFovy -= yoffset * 2;
	if (gFovy > 60) gFovy = 60;
	if (gFovy < 30) gFovy = 30;
	if (gFovy < 60 && gFovy > 30) {
		setUpProjection();
	}
}