#include "Lighting.h"

GLuint VAO = 0;
GLuint gVertexProgram, gWireframeProgram;

glm::mat4 gProjection = glm::mat4(1);
float gFovy = 45;

ObjectManager* gObjectManager;

int main(void)
{
	if (setUpOpenGlComponents() == -1) {
		getchar();
		return -1;
	}
	setUpData();
	setUpShaders();

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

	return 1;
}

void setUpData() {
	gObjectManager = ObjectManager::getInstance();
	setUpProjection();
}

void setUpProjection()
{
	gProjection = glm::perspective(glm::radians(gFovy), 4.0f / 3.0f, 0.1f, 100.0f);
	gObjectManager->setProjectionMatrix(gProjection);
}

void setUpShaders() {
	// vertex shader
	gVertexProgram = initShaders(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	glUseProgram(gVertexProgram);

	// position attribute
	GLint vPosition = glGetAttribLocation(gVertexProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

	// position attribute
	GLint vNormal = glGetAttribLocation(gVertexProgram, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// color attribute
	GLint vColor = glGetUniformLocation(gVertexProgram, "vColor");
	if (vColor < 0) {
		std::cerr << "couldn't find vColor in shader\n";
		exit(0);
	}
	gObjectManager->setColorLocation(vColor);

	// MVP location
	GLint mvpLocation = glGetUniformLocation(gVertexProgram, "MVP");
	if (mvpLocation < 0) {
		std::cerr << "couldn't find MVP in shader\n";
		exit(0);
	}
	gObjectManager->setMvpLocation(mvpLocation);

	// wireframe shader
	gWireframeProgram = initShaders(WIREFRAME_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
}

void renderWorld() { 
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			glUseProgram(gWireframeProgram);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLFW_KEY_R:
			gObjectManager->toggleRotation();
			break;
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
		case GLFW_KEY_5:
		case GLFW_KEY_6:
		case GLFW_KEY_7:
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