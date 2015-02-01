#include "AnimatedSpritePhysic.h"

AnimatedSpritePhysics::AnimatedSpritePhysics(){
	mBody = 0;
	mShape = 0;
}

AnimatedSpritePhysics::~AnimatedSpritePhysics(){
	Delete(mShape);
	b2Body* body = mBody;
	while (Global::gWorld->IsLocked());
	if (!Global::gWorld->IsLocked() && body != 0){
		Global::gWorld->DestroyBody(body);
	}
}

void AnimatedSpritePhysics::SetPosition(Vector3 value)
{
	Entity2D::SetPosition(value);
	if (mBody)
		mBody->SetTransform(b2Vec2(mPosition.x, mPosition.y), mRotateAngles.z);
}

void AnimatedSpritePhysics::SetScaleFactor(Vector3 value)
{
	Entity2D::SetScaleFactor(value);
	if (mBody){
		b2BodyType type = mBody->GetType();
		while ((*Global::gWorld).IsLocked())
		{

		}
		b2Shape::Type shape = mShape->GetType();
		Delete(mShape);
		(*Global::gWorld).DestroyBody(mBody);
		if (type == b2_staticBody)
		{
			CreateStaticBody(shape);
		}
		else if (type == b2_dynamicBody){
			CreateDynamicBody(shape, mDensity, mFriction, mRestitution, mSensor);
		}
	}
}

void AnimatedSpritePhysics::SetRotateAngles(Vector3 value)
{
	Entity2D::SetRotateAngles(value);
	if (mBody)
		mBody->SetTransform(b2Vec2(mPosition.x, mPosition.y), mRotateAngles.z);
}

void AnimatedSpritePhysics::update(float deltatime)
{
	AnimatedSprite::update(deltatime);
	if (mBody != 0 && mBody->GetType() == b2_dynamicBody){
		mPosition.x = mBody->GetPosition().x;
		mPosition.y = mBody->GetPosition().y;
		mRotateAngles.z = mBody->GetAngle();
	}
	else if (mBody != 0){
		mBody->SetTransform(b2Vec2(mPosition.x, mPosition.y), mRotateAngles.z);
	}
}

void AnimatedSpritePhysics::drawBound()
{
#ifdef Win32
	glUseProgram(Global::BoundShader->program);
#endif
#ifdef WindowPhone
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);

	m_d3dContext->IASetInputLayout(Global::BoundShader->m_inputLayout.Get());

	m_d3dContext->VSSetShader(
		Global::BoundShader->vertexShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->PSSetShader(
		Global::BoundShader->pixelShader.Get(),
		nullptr,
		0
		);
#endif
	Matrix MVP, world;
	MVP.SetIdentity();
	//world.SetIdentity();
	Matrix World;
	World.SetIdentity();
	//World.SetScale(10.0f);
	Matrix Translate = Translate.SetIdentity();
	if (mBody != 0)
		Translate.SetTranslation(Vector3(mBody->GetPosition().x, mBody->GetPosition().y, mPosition.z));
	else
		Translate.SetTranslation(mPosition);
	Matrix RotationX, RotationY, RotationZ;
	RotationX.SetIdentity();
	RotationY.SetIdentity();
	RotationZ.SetIdentity();
	RotationX.SetRotationX(mRotateAngles.x);
	RotationY.SetRotationY(mRotateAngles.y);
	RotationZ.SetRotationZ(mRotateAngles.z);
	Matrix Rotation = RotationZ*RotationX*RotationY;
	World = Rotation*Translate;
	MVP = World*Global::currentCamera->ViewMatrix() * Global::currentCamera->projection;
#ifdef Win32
	if (Global::BoundShader->u_MatrixMVP != -1)	// Send MVP
		glUniformMatrix4fv(Global::BoundShader->u_MatrixMVP, 1, GL_FALSE, (float*)&MVP);
#endif
#ifdef WindowPhone
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		Global::m_d3ddevice->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		&m_constantBuffer
		)
		);

	MVPConstantBuffer m_constantBufferData;
	m_constantBufferData.MVP = MVP.Transpose();

	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	m_constantBuffer.ReleaseAndGetAddressOf();
#endif
	GetBound().Draw(mPosition.z);
}


void AnimatedSpritePhysics::SetSizeOfBound(float width, float height){
	mBound.mWidth = width;
	mBound.mHeight = height;
	if (mBody){
		b2BodyType type = mBody->GetType();
		while ((*Global::gWorld).IsLocked())
		{

		}
		b2Shape::Type shape = mShape->GetType();
		Delete(mShape);
		(*Global::gWorld).DestroyBody(mBody);
		if (type == b2_staticBody)
		{
			CreateStaticBody(shape);
		}
		else if (type == b2_dynamicBody){
			CreateDynamicBody(shape, mDensity, mFriction, mRestitution, mSensor);
		}
	}
}

b2Body* AnimatedSpritePhysics::GetBody(){
	return mBody;
}

void AnimatedSpritePhysics::CreateStaticBody(b2Shape::Type shape){
	
	b2World* world = Global::gWorld;
	if (mBody != 0)
		(*world).DestroyBody(mBody);
	mBodyDef.position.Set(mPosition.x, mPosition.y);
	mBodyDef.angle = mRotateAngles.z;
	mBodyDef.type = b2_staticBody;

	switch (shape)
	{
	case b2Shape::e_circle:
		mShape = new b2CircleShape();
		((b2CircleShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		mBody->CreateFixture(mShape, 0.0f);
		break;
	case b2Shape::e_edge:
		mShape = new b2EdgeShape();
		((b2EdgeShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		mBody->CreateFixture(mShape, 0.0f);
		break;
	case b2Shape::e_polygon:
		mShape = new b2PolygonShape();
		((b2PolygonShape*)mShape)->SetAsBox(mBound.mWidth / 2, mBound.mHeight / 2);
		mBody = (*world).CreateBody(&mBodyDef);
		mBody->CreateFixture(mShape, 0.0f);
		break;
	case b2Shape::e_chain:
		mShape = new b2ChainShape();
		((b2ChainShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		mBody->CreateFixture(mShape, 0.0f);
		break;
	case b2Shape::e_typeCount:
		break;
	default:
		break;
	}
	mBody->SetUserData(this);
	mBound.mShape = mShape;
}

void AnimatedSpritePhysics::CreateKinematicBody(b2Shape::Type shape, float density, float friction, float restitution, bool sensor){

	b2World* world = Global::gWorld;
	if (mBody != 0)
		(*world).DestroyBody(mBody);
	b2FixtureDef fixtureDef;
	mBodyDef.position.Set(mPosition.x, mPosition.y);
	mBodyDef.angle = mRotateAngles.z;
	mBodyDef.type = b2_kinematicBody;

	mDensity = density;
	mFriction = friction;
	mRestitution = restitution;
	mSensor = sensor;
	switch (shape)
	{
	case b2Shape::e_circle:
		mShape = new b2CircleShape();
		((b2CircleShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_edge:
		mShape = new b2EdgeShape();
		((b2EdgeShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_polygon:
		mShape = new b2PolygonShape();
		((b2PolygonShape*)mShape)->SetAsBox(mBound.mWidth / 2, mBound.mHeight / 2);
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_chain:
		mShape = new b2ChainShape();
		((b2ChainShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_typeCount:
		break;
	default:
		break;
	}
	mBody->SetUserData(this);
	mBound.mShape = mShape;
}

void AnimatedSpritePhysics::CreateDynamicBody(b2Shape::Type shape, float density, float friction, float restitution, bool sensor){

	b2World* world = Global::gWorld;
	if (mBody != 0)
		(*world).DestroyBody(mBody);
	b2FixtureDef fixtureDef;
	mBodyDef.position.Set(mPosition.x, mPosition.y);
	mBodyDef.angle = mRotateAngles.z;
	mBodyDef.type = b2_dynamicBody;

	mDensity = density;
	mFriction = friction;
	mRestitution = restitution;
	mSensor = sensor;
	switch (shape)
	{
	case b2Shape::e_circle:
		mShape = new b2CircleShape();
		((b2CircleShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_edge:
		mShape = new b2EdgeShape();
		((b2EdgeShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_polygon:
		mShape = new b2PolygonShape();
		((b2PolygonShape*)mShape)->SetAsBox(mBound.mWidth / 2, mBound.mHeight / 2);
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_chain:
		mShape = new b2ChainShape();
		((b2ChainShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.isSensor - sensor;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_typeCount:
		break;
	default:
		break;
	}
	mBody->SetUserData(this);
	mBound.mShape = mShape;
}


void AnimatedSpritePhysics::CreateKinematicBody(b2Shape::Type shape){

	b2World* world = Global::gWorld;
	if (mBody != 0)
		(*world).DestroyBody(mBody);
	b2FixtureDef fixtureDef;
	mBodyDef.position.Set(mPosition.x, mPosition.y);
	mBodyDef.angle = mRotateAngles.z;
	mBodyDef.type = b2_kinematicBody;
	switch (shape)
	{
	case b2Shape::e_circle:
		mShape = new b2CircleShape();
		((b2CircleShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_edge:
		mShape = new b2EdgeShape();
		((b2EdgeShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_polygon:
		mShape = new b2PolygonShape();
		((b2PolygonShape*)mShape)->SetAsBox(mBound.mWidth / 2, mBound.mHeight / 2);
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_chain:
		mShape = new b2ChainShape();
		((b2ChainShape*)mShape)->m_radius = mBound.mWidth / 2;
		mBody = (*world).CreateBody(&mBodyDef);
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
		break;
	case b2Shape::e_typeCount:
		break;
	default:
		break;
	}
	mBody->SetUserData(this);
	mBound.mShape = mShape;
}

AnimatedSpritePhysics* AnimatedSpritePhysics::Create(){
	return new AnimatedSpritePhysics();
}

AnimatedSpritePhysics* AnimatedSpritePhysics::Clone(){
	AnimatedSpritePhysics* clone = Create();
	CloneAttribute(clone);
	return clone;
}