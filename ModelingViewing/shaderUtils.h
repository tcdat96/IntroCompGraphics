#pragma once

#include "utils.h"

#include <vector>

#include <glew/glew.h>

GLuint initShaders(const char* vertexShaderFile, const char* fragShaderFile);
static GLuint initShader(const char* source, GLenum type);