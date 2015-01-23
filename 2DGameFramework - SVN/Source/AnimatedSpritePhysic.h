#pragma once
#include "AnimatedSprite.h"

class AnimatedSpritePhysics :public AnimatedSprite{
protected:
	//Physic
	b2Body* mBody;
	b2BodyDef mBodyDef;
	b2Shape* mShape;
	float mDensity;
	float mFriction;
	float mRestitution;
	bool mSensor;
protected:
public:
	AnimatedSpritePhysics();
	~AnimatedSpritePhysics();
	b2Body* GetBody();
	void SetSizeOfBound(float width, float height);

	void SetPosition(Vector3);
	void SetScaleFactor(Vector3);
	void SetRotateAngles(Vector3);

	virtual void update(float);

	virtual void CreateStaticBody(b2Shape::Type shape);
	virtual void CreateDynamicBody(b2Shape::Type shape, float density, float friction, float restitution, bool sensor);
	virtual void CreateKinematicBody(b2Shape::Type shape);
	virtual void CreateKinematicBody(b2Shape::Type shape, float density, float friction, float restitution, bool sensor);
	virtual void drawBound();

	AnimatedSpritePhysics* Create();
	AnimatedSpritePhysics* Clone();
};