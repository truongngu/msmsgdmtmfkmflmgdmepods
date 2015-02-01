#include "LookAtCamera.h"

#pragma region Ctor
LookAtCamera::LookAtCamera(void)
{
	position=Vector3(0,0,0.0f);
	target=Vector3(0,0,0);
	up=Vector3(0,1,0);
	rotX=0;
	rotY=0;
	rotZ=0;
}

LookAtCamera::~LookAtCamera(void)
{
}

#pragma endregion Ctor

#pragma region Properties
void LookAtCamera::SetTarget(Vector3 newTarget)
{
	target=newTarget;
}
Vector3 LookAtCamera::GetTarget()
{
	return target;
}
void LookAtCamera::SetUpVector(Vector3 upVector)
{
	this->up = upVector;
}
Vector3 LookAtCamera::GetUpVector()
{
	return this->up;
}
#pragma endregion Properties

#pragma region Get Matrixes
Matrix LookAtCamera::ViewMatrix()
{
	Matrix InverseTranslate=InverseTranslate.SetIdentity().SetTranslation(-position);
	Vector3 zaxis=(position-target).Normalize();
	Vector3 xaxis=(up.Cross(zaxis)).Normalize();
	Vector3 yaxis=(zaxis.Cross(xaxis)).Normalize();

	Matrix Rotate=Rotate.SetIdentity();
	Rotate.m[0][0]=xaxis.x;
	Rotate.m[0][1]=xaxis.y;
	Rotate.m[0][2]=xaxis.z;
	Rotate.m[0][3]=0;

	Rotate.m[1][0]=yaxis.x;
	Rotate.m[1][1]=yaxis.y;
	Rotate.m[1][2]=yaxis.z;
	Rotate.m[1][3]=0;

	Rotate.m[2][0]=zaxis.x;
	Rotate.m[2][1]=zaxis.y;
	Rotate.m[2][2]=zaxis.z;
	Rotate.m[2][3]=0;

	Matrix InverseRotate=Rotate.Transpose();	//R^-1 == R^T

	return InverseTranslate*InverseRotate;
}

Matrix LookAtCamera::WorldMatrix()
{
	Matrix world;
	Matrix Translate=Translate.SetIdentity().SetTranslation(position);
	Vector3 zaxis=(position-target).Normalize();
	Vector3 xaxis=(up.Cross(zaxis)).Normalize();
	Vector3 yaxis=(zaxis.Cross(xaxis)).Normalize();

	Matrix Rotate=Rotate.SetIdentity();
	Rotate.m[0][0]=xaxis.x;
	Rotate.m[0][1]=xaxis.y;
	Rotate.m[0][2]=xaxis.z;
	Rotate.m[0][3]=0;

	Rotate.m[1][0]=yaxis.x;
	Rotate.m[1][1]=yaxis.y;
	Rotate.m[1][2]=yaxis.z;
	Rotate.m[1][3]=0;

	Rotate.m[2][0]=zaxis.x;
	Rotate.m[2][1]=zaxis.y;
	Rotate.m[2][2]=zaxis.z;
	Rotate.m[2][3] = 0;
	world=Rotate*Translate;
	return world;
}

#pragma endregion Get Matrixes

#pragma region Method 
void LookAtCamera::MoveForward(float offset)
{
	Vector3 deltaMove=-(position-target).Normalize()*offset;
	position+=deltaMove;
	target+=deltaMove;
}



void LookAtCamera::MoveBackward(float offset)
{
	Vector3 deltaMove=(position-target).Normalize()*offset;
	position+=deltaMove;
	target+=deltaMove;
}

void LookAtCamera::MoveLeft(float offset)
{
	Vector3 zAxis=(position-target).Normalize();
	Vector3 deltaMove=-(up.Cross(zAxis)).Normalize()*offset;
	position+=deltaMove;
	target+=deltaMove;
}

void LookAtCamera::MoveRight(float offset)
{
	Vector3 zAxis=(position-target).Normalize();
	Vector3 deltaMove=(up.Cross(zAxis)).Normalize()*offset;
	position+=deltaMove;
	target+=deltaMove;
}


void LookAtCamera::RotateUp(float offset)
{
	rotX+=offset;
	//if (rotX>3.14f/2)
	//{
	//	//rotX=-3.14f/2;
	//	return;
	//}
	Vector4 localTarget=Vector4(0,0,-(position-target).Length(),1);
	Matrix RotateAroundX=RotateAroundX.SetIdentity().SetRotationX(offset);
	Vector4 localNewTarget=localTarget*RotateAroundX;
	Vector4 newTarget=localNewTarget*WorldMatrix();
	target=Vector3(newTarget.x,newTarget.y,newTarget.z);
}

void LookAtCamera::RotateDown(float offset)
{
	/**/
	rotX-=offset;
	//if (rotX<3.14f/2)
	//{
	//	//rotX=3.14f/2;
	//	return;
	//}
	Vector4 localTarget=Vector4(0,0,-(position-target).Length(),1);
	Matrix RotateAroundX=RotateAroundX.SetIdentity().SetRotationX(-offset);
	Vector4 localNewTarget=localTarget*RotateAroundX;
	Vector4 newTarget=localNewTarget*WorldMatrix();
	target=Vector3(newTarget.x,newTarget.y,newTarget.z);
}

void LookAtCamera::RotateLeft(float offset)
{
	Vector4 localTarget=Vector4(0,0,-(position-target).Length(),1);
	Matrix RotateAroundY=RotateAroundY.SetIdentity().SetRotationY(offset);
	Vector4 localNewTarget=localTarget*RotateAroundY;
	Matrix world=world.SetIdentity();
	Vector4 newTarget=localNewTarget*WorldMatrix();
	target=Vector3(newTarget.x,newTarget.y,newTarget.z);
}

void LookAtCamera::RotateRight(float offset)
{
	Vector4 localTarget=Vector4(0,0,-(position-target).Length(),1);
	Matrix RotateAroundY=RotateAroundY.SetRotationY(-offset);
	Vector4 localNewTarget=localTarget*RotateAroundY;
	Matrix world=world.SetIdentity();
	Vector4 newTarget=localNewTarget*WorldMatrix();
	target=Vector3(newTarget.x,newTarget.y,newTarget.z);
}




#pragma endregion Method
