#pragma once
#include <string>
#include "Vertex.h"
#include "Shader.h"
#include "Resource.h"
using namespace std;

enum MeshRenderType
{
	RenderByArray=0,
	RenderByIndex
};

class Mesh2D:public Resource
{
private:
protected:
	GLuint vbo;
	int mNumVertices;
	GLuint ido;
	int mNumIndex;
	MeshRenderType mRenderType;
	GLenum mDrawMode;
public:
	Mesh2D();
	~Mesh2D();
	string GetType(){return string("Mesh");}
	void Load(string path2Mesh, MeshRenderType type);
	void Render(Shaders* sha);
	void Unrender();
};