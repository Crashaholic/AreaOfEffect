#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	Mesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f);
	Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	Mesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);
};

#endif