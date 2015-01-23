#include "Camera.h"

#pragma region Properties

#pragma endregion Properties

#pragma region Method Treat Movement
void Camera::MoveForward(float offset)
{
	Vector4 moveL=Vector4(0,0,-offset,1);
	//Matrix World;
	//World.SetIdentity();
	Vector4 moveW=moveL*WorldMatrix();	//
	//this->position=Vector3(this->position.x+moveW.x,this->position.y+moveW.y,this->position.z+moveW.z);
	this->position=Vector3(moveW.x,moveW.y,moveW.z);
}

void Camera::MoveBackward(float offset)
{
	Vector4 moveL=Vector4(0,0,offset,1);
	Matrix World;
	World.SetIdentity();
	Vector4 moveW=moveL*WorldMatrix();
	//this->position=Vector3(this->position.x+moveW.x,this->position.y+moveW.y,this->position.z+moveW.z);
	this->position=Vector3(moveW.x,moveW.y,moveW.z);
}

void Camera::MoveLeft(float offset)
{
	Vector4 moveL=Vector4(-offset,0,0,1);
	Matrix World;
	World.SetIdentity();
	Vector4 moveW=moveL*WorldMatrix();
	//this->position=Vector3(this->position.x-moveW.x,this->position.y-moveW.y,this->position.z-moveW.z);
	this->position=Vector3(moveW.x,moveW.y,moveW.z);
}

void Camera::MoveRight(float offset)
{
	Vector4 moveL=Vector4(offset,0,0,1);
	Matrix World;
	World.SetIdentity();
	Vector4 moveW=moveL*WorldMatrix();
	//this->position=Vector3(this->position.x-moveW.x,this->position.y-moveW.y,this->position.z-moveW.z);
	this->position=Vector3(moveW.x,moveW.y,moveW.z);
}

void Camera::RotateUp(float angle)
{
	this->rotX+=angle;
	if (this->rotX>=360.0f)
		this->rotX-=360.0f;
}

void Camera::RotateDown(float angle)
{
	rotX-=angle;
	if (this->rotX<=-360.0f)
		this->rotX=360.0f;
}

void Camera::RotateLeft(float angle)
{
	rotY+=angle;
	if (this->rotY<=-360.0f)
		this->rotY=360.0f;
}

void Camera::RotateRight(float angle)
{
	rotY-=angle;
	if (this->rotY>=360.0f)
		this->rotY-=360.0f;
}

void Camera::MoveForwardOnTerrain(float offset)
{

}
#pragma endregion Method Treat Movement

#pragma region Method Get Matrixes
Matrix Camera::ViewMatrix()
{
	Matrix Translate;
	Translate.SetIdentity();
	Translate.SetTranslation(-this->position.x,-this->position.y,-this->position.z);

	Matrix RotationX,RotationY,RotationZ;
	RotationX.SetIdentity();
	RotationY.SetIdentity();
	RotationZ.SetIdentity();
	RotationX.SetRotationX(-rotX);
	RotationY.SetRotationY(-rotY);
	RotationZ.SetRotationZ(-rotZ);

	Matrix Rotation=RotationY*RotationX*RotationZ;
	return Translate*Rotation;
}

Matrix Camera::WorldMatrix()
{
	Matrix World;
	World.SetIdentity();
	//World.SetScale(10.0f);
	Matrix Translate=Translate.SetIdentity();
	Translate.SetTranslation(position);
	Matrix RotationX,RotationY,RotationZ;
	RotationX.SetIdentity();
	RotationY.SetIdentity();
	RotationZ.SetIdentity();
	RotationX.SetRotationX(rotX);
	RotationY.SetRotationY(rotY);
	RotationZ.SetRotationZ(rotZ);
	Matrix Rotation=RotationZ*RotationX*RotationY;

	return Rotation*Translate;
}

#pragma endregion Method Get Matrixes