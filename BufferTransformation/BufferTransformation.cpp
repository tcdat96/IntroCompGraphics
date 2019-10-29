#include "BufferTransformation.h"

GLuint VAO = 0;
GLuint VBO = 0;
GLuint gVertexProgram, gEdgeProgram;

GLint gObjectToWorldUniformLocation;

GLfloat* vPositions;
GLfloat* vColors;
std::vector<unsigned int> gIndices;
int gVerticesCount = 0;

int gPyramidIndices[3];

float gRotAngle = 0;
TranslateMode translateMode = TranslateMode::NONE;
float gTranslationDx = 0;
bool gLeftToRight = true;

int main(void)
{
	int status = setup();
	if (status == -1) {
		getchar();
		return -1;
	}

	setUpData();

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	int vSize = gVerticesCount * 3 * sizeof(GLfloat);
	glBufferData(GL_ARRAY_BUFFER, vSize * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vPositions);
	glBufferSubData(GL_ARRAY_BUFFER, vSize, vSize, vColors);

	// IBO
	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gIndices.size() * sizeof(unsigned int), &gIndices[0], GL_STATIC_DRAW);

	// vertex shader
	const char* vertexShaderSrc = "vertexShader.glsl";
	const char* fragShaderSrc = "fragShader.glsl";
	gVertexProgram = initShaders(vertexShaderSrc, fragShaderSrc);

	GLint vPosition = glGetAttribLocation(gVertexProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLint vColor = glGetAttribLocation(gVertexProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)(gVerticesCount * 3 * sizeof(GLfloat)));

	gObjectToWorldUniformLocation = glGetUniformLocation(gVertexProgram, "objectToWorld");
	if (gObjectToWorldUniformLocation < 0) std::cerr << "couldn't find ObjectToWorld in shader\n";

	// edge shader
	const char* edgeShaderSrc = "edgeShader.glsl";
	gEdgeProgram = initShaders(edgeShaderSrc, fragShaderSrc);
	GLint edgeColorUniformLocation = glGetUniformLocation(gEdgeProgram, "vEdgeColor");
	if (edgeColorUniformLocation != -1) {
		glUniform3f(edgeColorUniformLocation, 0, 0, 0);
	}

	do {
		renderWorld();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();

	delete vPositions;
	delete vColors;

	return 0;
}

int setup() {
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
	int depth = 5;
	GLfloat length = 0.20f;
	int pyramidCount = depth * (depth + 1) / 2;
	gVerticesCount = pyramidCount + /*rotating one*/ 3 + /*first triangle*/ 3;
	
	int posCount = gVerticesCount * 3;
	vPositions = new GLfloat[posCount];
	vColors = new GLfloat[posCount];

	// generate pyramid vertices
	vPositions[1] = 0.4;
	vPositions[0] = vPositions[2] = 0;
	for (int i = 2, idx = 3; i <= depth; i++) {
		GLfloat x = vPositions[0] - length / 2 * (i - 1);
		for (int j = 0; j < i; j++) {
			vPositions[idx++] = x;
			vPositions[idx++] = vPositions[1] - length * (i - 1);
			vPositions[idx++] = 0;
			x += length;
		}
	}

	// 3 vertices of the pyramid
	gPyramidIndices[0] = 0;
	gPyramidIndices[1] = 10;
	gPyramidIndices[2] = 14;

	// generate rotating triangle
	int offset = pyramidCount * 3;
	vPositions[offset] = vPositions[0];
	vPositions[offset + 1] = vPositions[1] + 0.2f;
	vPositions[offset + 3] = vPositions[offset] - length / 2;
	vPositions[offset + 6] = vPositions[offset] + length / 2;
	vPositions[offset + 4] = vPositions[offset + 7] = vPositions[offset + 1] + length;
	vPositions[offset + 2] = vPositions[offset + 5] = vPositions[offset + 8] = 0;

	// first triangle
	offset += 9;
	GLfloat firstTri[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	for (int i = 0; i < 9; i++) {
		vPositions[offset + i] = firstTri[i];
	}

	unsigned int idx[] = {
		// first triangle
		18,19,20,
		// pyramids
		0,1,2,
		1,3,4, 1,4,2, 2,4,5,
		3,6,7, 3,7,4, 4,7,8, 4,8,5, 5,8,9,
		6,10,11, 6,11,7, 7,11,12, 7,12,8, 8,12,13, 8,13,9, 9,13,14,
		// rotating
		15,16,17
	};
	std::copy(&idx[0], &idx[sizeof(idx) / sizeof(unsigned int)], std::back_inserter(gIndices));

	generateVerticesColor();
}

void generateVerticesColor() {
	for (int i = 0; i < gVerticesCount; i++) {
		setVertexColor(i, 
			(float)rand() / RAND_MAX, 
			(float)rand() / RAND_MAX, 
			(float)rand() / RAND_MAX
		);
	}

	// solid triangles
	GLint solids[] = { 0,1,2, 4,7,8, 6,10,11, 9,13,14, 15,16,17 };
	int solidCount = sizeof(solids) / sizeof(GLint);
	for (int i = 0; i < solidCount;) {
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		setVertexColor(solids[i++], r, g, b);
		setVertexColor(solids[i++], r, g, b);
		setVertexColor(solids[i++], r, g, b);
	}

	// first triangle
	int offset = gVerticesCount - 3;
	setVertexColor(offset, .0f, 1.0f, .0f);
	setVertexColor(offset + 1, .0f, 1.0f, .0f);
	setVertexColor(offset + 2, .0f, 1.0f, .0f);
}

void setVertexColor(int triIdx, float r, float g, float b) {
	int offset = triIdx * 3;
	vColors[offset] = r;
	vColors[offset + 1] = g;
	vColors[offset + 2] = b;
}

GLuint initShaders(const char* vertexShaderFile, const char* fragShaderFile) {
	GLuint program = glCreateProgram();

	const char* vertexShaderSrc = readFile(vertexShaderFile);
	if (vertexShaderSrc != NULL) {
		GLuint vertexShader = initShader(vertexShaderSrc, GL_VERTEX_SHADER);
		glAttachShader(program, vertexShader);
		delete vertexShaderSrc;
	}

	const char* fragShaderSrc = readFile(fragShaderFile);
	if (fragShaderSrc != NULL) {
		GLuint fragShader = initShader(fragShaderSrc, GL_FRAGMENT_SHADER);
		glAttachShader(program, fragShader);
		delete fragShaderSrc;
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


static GLuint initShader(const char* source, GLenum type) {
	GLuint shader = glCreateShader(type);
	try {
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
	}
	catch (int e) {
		std::cout << "Exception: " << e << std::endl;
	}

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

void renderWorld() { 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// filled
	glUseProgram(gVertexProgram);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderTriangles();

	// wireframe
	glUseProgram(gEdgeProgram);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	renderTriangles();
}

void renderTriangles() {
	updateTransformValue();

	mat4 Xfm = mat4(1);
	glUniformMatrix4fv(gObjectToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(Xfm));
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

	// pyramids
	if (gTranslationDx > 0) Xfm = translate(mat4(1), vec3(gTranslationDx, 0, 0));
	glUniformMatrix4fv(gObjectToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(Xfm));
	glDrawElements(GL_TRIANGLES, gIndices.size() - 6, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLfloat)));

	// rotating triangle
	Xfm = glm::rotate(Xfm, gRotAngle, glm::vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(gObjectToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(Xfm));
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)((gIndices.size() - 3) * sizeof(GLfloat)));
}

static void updateTransformValue() {
	if (translateMode == TranslateMode::DRAGGING) {
		double x, y;
		getCursorPos(x, y);
		gTranslationDx = (float)std::min(std::max(x, 0.0), TRANSLATE_LIMIT);
	}
	else if (translateMode == TranslateMode::AUTO) {
		if (gLeftToRight) {
			gTranslationDx += TRANSLATE_DELTA;
			if (gTranslationDx > TRANSLATE_LIMIT) gLeftToRight = false;
		}
		else {
			gTranslationDx -= TRANSLATE_DELTA;
			if (gTranslationDx <= 0) {
				gTranslationDx = TRANSLATE_DELTA;
				gLeftToRight = true;
			}
		}
	}

	if (gRotAngle >= 0) {
		gRotAngle += ROTATE_ANGLE_DELTA;
		if (gRotAngle > M_PI * 2) gRotAngle = 0;
	}
}

void updateVerticesColor() {
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vPositions), sizeof(vColors), vColors);
}


// input handler
void getCursorPos(double& x, double& y) {
	glfwGetCursorPos(window, &x, &y);
	x = (x * 2 - SCREEN_WIDTH) / SCREEN_WIDTH;
	y = -(y * 2 - SCREEN_HEIGHT) / SCREEN_HEIGHT;
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS) {
			double xpos, ypos;
			getCursorPos(xpos, ypos);

			bool inPyramid = inTriangle(
				vPositions[gPyramidIndices[0] * 3] + gTranslationDx, vPositions[gPyramidIndices[0] * 3 + 1],
				vPositions[gPyramidIndices[1] * 3] + gTranslationDx, vPositions[gPyramidIndices[1] * 3 + 1],
				vPositions[gPyramidIndices[2] * 3] + gTranslationDx, vPositions[gPyramidIndices[2] * 3 + 1],
				(float)xpos, (float)ypos
			);

			if (inPyramid) {
				translateMode = TranslateMode::DRAGGING;
			}
			else {
				generateVerticesColor();
				int vSize = gVerticesCount * 3 * sizeof(GLfloat);
				glBufferSubData(GL_ARRAY_BUFFER, vSize, vSize, vColors);
			}
		} 
		else if (action == GLFW_RELEASE) {
			translateMode = TranslateMode::NONE;
		}
	} 
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		switch (translateMode)
		{
		case TranslateMode::NONE:
			translateMode = TranslateMode::AUTO;
			break;
		case TranslateMode::AUTO:
			translateMode = TranslateMode::NONE;
			gTranslationDx = 0; 
			break;
		case TranslateMode::DRAGGING:
			translateMode = TranslateMode::NONE;
			break;
		}
	}
}