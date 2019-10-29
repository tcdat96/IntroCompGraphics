// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>

// Include GLEW
#include <glew/glew.h>

// Include GLFW
#include <glfw/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

GLuint VAO = 0;
GLuint VBO = 0;

int scrWidth = 1024;
int scrHeight = 768;

int setup();
void setUpData();
GLuint initShaders(const char* vertexShaderFile, const char* fragShaderFile);
static char* readFile(const char* fileName);
static GLuint initShader(const char* source, GLenum type);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);

GLfloat vColors[] = {
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0
};

GLfloat vPositions[] = {
	0.0, -0.25, 0.0,
	-0.25, 0.25, 0.0,
	0.25, 0.25, 0.0,

	-0.25, 0.25, 0.0,
	0.0, -0.25, 0.0,
	-0.5, -0.25, 0.0,

	0.25, 0.25, 0.0,
	0.0, -0.25, 0.0,
	0.5, -0.25, 0.0
};

int main(void)
{
	int status = setup();
	if (status == -1) {
		getchar();
		return -1;
	}

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions) + sizeof(vColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vPositions), vPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vPositions), sizeof(vColors), vColors);

	const char* vertexShaderSrc = "vertexShader.glsl";
	const char* fragShaderSrc = "fragShader.glsl";
	GLuint program = initShaders(vertexShaderSrc, fragShaderSrc);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	do {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vPositions), sizeof(vColors), vColors);

		GLint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		GLint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vPositions));

		glDrawArrays(GL_TRIANGLES, 0, 9);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();

	return 0;
}

int setup() {
	std::srand(std::time(nullptr));

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
	window = glfwCreateWindow(scrWidth, scrHeight, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// interaction
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetMouseButtonCallback(window, mouseCallback);

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

}

GLuint initShaders(const char* vertexShaderFile, const char* fragShaderFile) {
	GLuint program = glCreateProgram();

	const char* vertexShaderSrc = readFile(vertexShaderFile);
	if (vertexShaderSrc != NULL) {
		GLuint vertexShader = initShader(vertexShaderSrc, GL_VERTEX_SHADER);
		glAttachShader(program, vertexShader);
	}

	const char* fragShaderSrc = readFile(fragShaderFile);
	if (fragShaderSrc != NULL) {
		GLuint fragShader = initShader(fragShaderSrc, GL_FRAGMENT_SHADER);
		glAttachShader(program, fragShader);
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> infoLog(logLength + 1);
			glGetProgramInfoLog(program, logLength, NULL, &infoLog[0]);
			std::cout << &infoLog[0] << std::endl;
		}
		exit(EXIT_FAILURE);
	}

	return program;
}

static char* readFile(const char* fileName) {
	FILE* fp = NULL;
	char* buf = NULL;
	errno_t err = fopen_s(&fp, fileName, "r");
	if (err != 0) {
		std::cout << "There was a problem opening " << fileName << "!" << std::endl;
	}
	else {
		if (fp == NULL) { return NULL; }
		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		buf = new char[size + 1];
		fread(buf, 1, size, fp);
		buf[size] = '\0';
		fclose(fp);
	}
	return buf;
}

static GLuint initShader(const char* source, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> infoLog(logLength + 1);
			glGetShaderInfoLog(shader, logLength, NULL, &infoLog[0]);
			std::cout << &infoLog[0] << std::endl;
		}
		exit(EXIT_FAILURE);
	}

	return shader;
}


// for mouse interaction
float sign(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

bool inTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(x, y, x1, y1, x2, y2);
	d2 = sign(x, y, x2, y2, x3, y3);
	d3 = sign(x, y, x3, y3, x1, y1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = (xpos * 2 - scrWidth) / scrWidth;
		ypos = -(ypos * 2 - scrHeight) / scrHeight;

		int vertexCount = sizeof(vPositions) / sizeof(GLfloat);
		for (int i = 0; i < vertexCount; i += 9) {
			if (inTriangle(vPositions[i], vPositions[i + 1], vPositions[i + 3], vPositions[i + 4], vPositions[i + 6], vPositions[i + 7],
				xpos, ypos)) {
				for (int j = 0; j < 3; j++) {
					int idx = i + std::rand() % 6;
					vColors[idx] = vColors[idx] > 0.5 ? 0.0 : 1.0;
				}
				break;
			}
		}
	}
}