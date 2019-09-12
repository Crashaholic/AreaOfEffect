#ifndef LOAD_TGA_H
#define LOAD_TGA_H

typedef unsigned int GLuint;

GLuint LoadTGA(const char *file_path, bool NearestNeighbour = true);

namespace Load
{
	GLuint TGA(const char* file_path, bool NearestNeighbour = true);
}

#endif