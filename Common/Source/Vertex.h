#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"

struct Position : public vec3
{
};

struct Color : public vec3
{
	float* r = &x;
	float* g = &y;
	float* b = &z;
};

struct TexCoord : public vec3
{
	float* u = &x;
	float* v = &y;
};

struct Vertex 
{
	vec3 pos;
	vec3 color;
	vec3 normal;
	vec3 texCoord;
	Vertex(){}
};

#endif