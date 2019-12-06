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

#include "ObjectManager.h"
#include "utils.h"
#include "shaderUtils.h"

using namespace std;

// Include GLEW
#include <glew/glew.h>

// Include GLFW
#include <glfw/glfw3.h>
GLFWwindow* window;

// constant
constexpr auto SCREEN_WIDTH = 1440;
constexpr auto SCREEN_HEIGHT = 900;

constexpr const char* SHADER_VERTEX = "VertexShader.glsl";
constexpr const char* SHADER_FRAG = "FragShader.glsl";

enum class TranslateMode {NONE, AUTO, DRAGGING};

int main(void);

// function declaration
int setUpOpenGlComponents();
void setUpData();
void setUpProjection();
void setUpShaders();
void renderWorld();

void getCursorPos(double& x, double& y);
bool isKeyPressed(int key);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);