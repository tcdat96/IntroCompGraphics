#pragma once

#define _USE_MATH_DEFINES

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <ctime>

#include "utils.h"

using namespace std;

// Include GLEW
#include <glew/glew.h>

// Include GLFW
#include <glfw/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// constant
constexpr auto SCREEN_WIDTH = 1024;
constexpr auto SCREEN_HEIGHT = 768;
constexpr auto TRANSLATE_LIMIT = 0.5;
constexpr float ROTATE_ANGLE_DELTA = 0.0005f;
constexpr float TRANSLATE_DELTA = 0.0001f;

enum class TranslateMode {NONE, AUTO, DRAGGING};

// function declaration
int setup();
void setUpData();
void generateVerticesColor();
void setVertexColor(int triIdx, float r, float g, float b);
GLuint initShaders(const char* vertexShaderFile, const char* fragShaderFile);
static GLuint initShader(const char* source, GLenum type);
void renderWorld();
void renderTriangles();
static void updateTransformValue();

void getCursorPos(double& x, double& y);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);