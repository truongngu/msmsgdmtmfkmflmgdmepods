#pragma once
#include "Mesh2D.h"

class RectangleMesh:public Mesh2D
{
private:
	static GLuint RectangleMeh_vbo;
	static GLuint RectangleMesh_ido;
	static int ReferenceCount;
protected:
public:
	RectangleMesh(float width,float height,float uvOffsetX,float uvOffsetY);
	~RectangleMesh();
};