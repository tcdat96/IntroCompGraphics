#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <chrono>

#include "Utils.h"
#include "Sphere.h"

#include <vector>

//#define DEBUG

#define DEFAULT_REFLECTION_COEFFICENT 0.1f
#define MAX_RAY_DEPTH 2
#define DEFAULT_SAMPLING_SUBDIVISION 2
#define MAX_SAMPLING_SUBDIVISION 3

using namespace std;

void castRays();
void castRaysSuperSampling();
void castRaysAdaptiveSuperSampling();
dvec3 castRayAdaptive(float left, float top, float size, int depth = 0);
dvec3 traceAdaptive(Ray& ray, float x, float y);
dvec3 trace(Ray& ray, float x, float y);
dvec3 trace(const Ray& ray);
Surface* findClosestIntersection(const Ray& ray);
dvec3 shade(const Ray& ray, Surface* surface);
bool isShadow(dvec3 hitPoint, const Light& light);
dvec3 PhongIllumination(const Ray& ray, Surface* surface, const Light& light);
dvec3 calcRefraction(const Ray& ray, Surface* surface, Refraction* curRef);

void printProgress(int i, int j);
void cleanUp();
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
void material(float dr, float dg, float db, float sr, float sg, float sb, float p, float kr);
void refraction(float r, float g, float b, float i);
void texture(int mode);
void antiAlias(int mode, int subdivision);