#pragma once

#include "globals.h"
#include <SDL.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct mat4x4
{
	float m[4][4] = { 0 };
};

struct vec3d
{
	float x, y, z;
	vec3d(float _x, float _y, float _z);
	vec3d();
	vec3d operator-(vec3d const& obj);
	vec3d operator+(vec3d const& obj);
	void operator-=(vec3d const& obj);
	void operator+=(vec3d const& obj);
	void normalize();
};

struct triangle
{
	vec3d p[3];
	vec3d normalVector();
	SDL_Color color;
};

struct mesh
{
	std::vector<triangle> tri;
	SDL_Color color = { 0,255,0 };
	bool LoadOBJ(std::string fileName);
	void move(float x, float y, float z);
};

vec3d rotateX(vec3d point, float angle);
vec3d rotateY(vec3d point, float angle);
vec3d rotateZ(vec3d point, float angle);
vec3d MultiplyMatrixVector(vec3d i, mat4x4 m);
vec3d project(vec3d point, float FOV);
float dotProduct(vec3d u, vec3d v);