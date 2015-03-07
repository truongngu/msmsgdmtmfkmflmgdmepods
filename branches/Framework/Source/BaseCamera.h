#pragma once
#include "Utilities\utilities.h"


class BaseCamera
{
protected:
	Vector3 position;
	float rotX;
	float rotY;
	float rotZ;
	bool transform;
	Matrix mWorldMatrix;
	Matrix mViewMatrix;
	bool isOrthorView;

public:
	BaseCamera(void);
	virtual ~BaseCamera(void);
	
	virtual Vector3 GetPosition();
	virtual Vector3 GetRotationAngle();
	virtual void SetPosition(Vector3);
	virtual void SetRotationAngle(Vector3);
	void SetOrthorProject(float left, float right, float bottom, float top,float zNear,float zFar);
	bool IsOrthorView(){return isOrthorView;}
	void SetPerspectiveProject(float anlgeY,float aspect,float zNear,float zFar);
	virtual void MoveForward(float)=0;

	virtual void MoveBackward(float)=0;
	virtual void MoveLeft(float)=0;
	virtual void MoveRight(float)=0;

	virtual void RotateUp(float)=0;
	virtual void RotateDown(float)=0;
	virtual void RotateLeft(float)=0;
	virtual void RotateRight(float)=0;

	virtual Matrix WorldMatrix()=0;
	virtual Matrix ViewMatrix()=0;
	Matrix projection;
};
