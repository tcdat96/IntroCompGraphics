#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string_view>

#include "Utils.h"

using namespace std;

void readScene(string filename);

void setView(float nPixel, float distance);

void sphere();

void scale(float sx, float sy, float sz);
void move(float x, float y, float z);
void rotate(float angle, float x, float y, float z);

void light(float r, float g, float b, float x, float y, float z);

void background(float r, float g, float b);
void ambient(float r, float g, float b);

void material(float dr, float dg, float db, float sr, float sg, float sb, float p);

void refraction(float r, float g, float b, float i);