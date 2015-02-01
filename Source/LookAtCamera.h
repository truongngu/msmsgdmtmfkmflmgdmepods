#pragma once
#include "BaseCamera.h"

class LookAtCamera:public BaseCamera
{
private:
	Vector3 target;
	Vector3 up;
	Vector3 localTarget;
public:
	LookAtCamera(void);
	~LookAtCamera(void);
	void SetTarget(Vector3);
	Vector3 GetTarget();
	void SetUpVector(Vector3);
	Vector3 GetUpVector();
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);

	void RotateUp(float);
	void RotateDown(float);
	void RotateLeft(float);
	void RotateRight(float);

	Matrix WorldMatrix();
	Matrix ViewMatrix();


};
