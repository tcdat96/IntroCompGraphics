#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string_view>

#include "Utils.h"
#include "Sphere.h"

#include <vector>

//#define DEBUG

using namespace std;

dvec3 trace(Ray& ray);
void findClosestIntersection(Ray& ray);
dvec3 shade(const Ray& ray);
bool isShadow(dvec3 hitPoint, const Ray& ray, Light light);
dvec3 PhongIllumination(dvec3 position, const Ray& ray, Light light);

bool readScene(string filename);
void exportPpm(dvec3** pixels, int xSize, int ySize);

void setView(int nPixel, float distance);
void sphere();
void scale(float sx, float sy, float sz);
void move(float x, float y, float z);
void rotate(float angle, float x, float y, float z);
void light(float r, float g, float b, float x, float y, float z);
void background(float r, float g, float b);
void ambient(float r, float g, float b);
void material(float dr, float dg, float db, float sr, float sg, float sb, float p);
void refraction(float r, float g, float b, float i);