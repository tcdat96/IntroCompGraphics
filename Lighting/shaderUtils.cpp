#include "shaderUtils.h"

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

GLint getUniformLocation(GLuint program, const GLchar* name)
{
	GLint location = glGetUniformLocation(program, name);
	if (location < 0) {
		std::cerr << "Couldn't find " << name << " in shader\n";
		exit(0);
	}
	return location;
}
