#pragma once
#include "BaseCamera.h"

class Camera:public BaseCamera
{
public:
	Camera()
	{
		position=Vector3(0,0,10.0f);
		rotX=0.0f;
		rotY=0.0f;
		rotZ=0.0f;
		projection.SetIdentity();
	}
	/*Vector3 GetPosition();
	Vector3 GetRotationAngle();
	void SetPosition(Vector3);
	void SetRotationAngle(Vector3);*/

	

	void MoveForward(float);
	void MoveForwardOnTerrain(float);
	void MoveBackward(float);
	void MoveLeft(float);
	void MoveRight(float);
	
	void RotateUp(float);
	void RotateDown(float);
	void RotateLeft(float);
	void RotateRight(float);

	Matrix WorldMatrix();
	Matrix ViewMatrix();
};