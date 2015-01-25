#include "Model2D.h"

Model2D::Model2D(Mesh2D* mesh2D, Texture* texture)
{
	mMesh2D=mesh2D;
	mTexture=texture;
	mesh2D->Refenrence();
	mTexture->Refenrence();
}

Model2D::~Model2D()
{
	mMesh2D->UnRefenrence();
	if (mMesh2D->GetReference()<=0)
		Delete(mMesh2D);
	mTexture->UnRefenrence();
	if(mTexture->GetReference()<=0)
		Delete(mTexture);
}

void Model2D::Draw(Shaders* shader)
{
	if (mTexture)
		mTexture->Render(shader);
	mMesh2D->Render(shader);
	mTexture->Unrender();
}