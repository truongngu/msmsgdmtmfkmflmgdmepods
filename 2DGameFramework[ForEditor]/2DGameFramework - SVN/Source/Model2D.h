#pragma once
#include "Mesh2D.h"
#include "Texture.h"


class Model2D
{
private:
	Mesh2D* mMesh2D;
	Texture* mTexture;
protected:
public:
	//Mesh Lay tu ResourceManager va Texture cung lay tu ResourceManager
	Model2D(Mesh2D*,Texture*);
	~Model2D();
	Mesh2D* GetMesh(){return mMesh2D;}
	Texture* GetTexture(){return mTexture;}
	void Draw(Shaders* shader);
};
