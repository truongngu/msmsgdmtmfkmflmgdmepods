#include "BaseCamera.h"

BaseCamera::BaseCamera(void)
{
}

BaseCamera::~BaseCamera(void)
{
}


Vector3 BaseCamera::GetPosition()
{
	return position;
}
Vector3 BaseCamera::GetRotationAngle()
{
	return Vector3(rotX,rotY,rotZ);
}
void BaseCamera::SetPosition(Vector3 val)
{
	position=val;
}

void BaseCamera::SetRotationAngle(Vector3 rotate)
{
	rotX=rotate.x;
	rotY=rotate.y;
	rotZ=rotate.z;
}

void BaseCamera::SetOrthorProject(float left, float right, float bottom, float top,float zNear,float zFar)
{
	projection.SetIdentity();
	projection.m[0][0]=2/(right-left);
	projection.m[0][3]=-(right+left)/(right-left);
	projection.m[1][1]=2/(top-bottom);
	projection.m[1][3]=-(top+bottom)/(top-bottom);
	projection.m[2][2]=-2/(zFar-zNear);
	projection.m[2][3]=-(zFar+zNear)/(zFar-zNear);
	isOrthorView=true;
}

void BaseCamera::SetPerspectiveProject(float angleY,float aspect,float zNear,float zFar)
{
	projection.SetPerspective(angleY,aspect,zNear,zFar);
	isOrthorView=false;
}