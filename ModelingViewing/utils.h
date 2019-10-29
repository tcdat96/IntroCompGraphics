#pragma once

#include <iostream>
#include <fstream>

float rand(float start, float end);
char* readFile(const char* fileName);
bool inTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);