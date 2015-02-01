#include "Entity2D.h"
#include "Global.h"
#include "DebugDefine.h"
#include "Utils.h"


float Entity2D::maxDepthOffset=1000000;
float Entity2D::giatoc = 0.5f;
float Entity2D::trongtruong = 9.8f;

#ifdef WindowPhone
void Bound::Draw(float depth){
	Vertex* verticesData = 0;
	float width = mWidth;
	float height = mHeight;
	float x = 0, y = 0, z = 0;
	int numVertices = 0;
	if (mShape == 0){
		verticesData = new Vertex[6];
		verticesData[0].position.x = x - width / 2;  verticesData[0].position.y = y + height / 2;  verticesData[0].position.z = z;
		verticesData[1].position.x = x - width / 2;  verticesData[1].position.y = y - height / 2;  verticesData[1].position.z = z;
		verticesData[2].position.x = x + width / 2;  verticesData[2].position.y = y - height / 2;  verticesData[2].position.z = z;

		verticesData[3].position.x = x - width / 2;  verticesData[3].position.y = y + height / 2;  verticesData[3].position.z = z;
		verticesData[4].position.x = x + width / 2;  verticesData[4].position.y = y + height / 2;  verticesData[4].position.z = z;
		verticesData[5].position.x = x + width / 2;  verticesData[5].position.y = y - height / 2;  verticesData[5].position.z = z;
		numVertices = 6;
	}
	else{
		float angle = 0.0f;
		float inc = 2 * PI / 100;
		switch (mShape->GetType())
		{
		case b2Shape::e_circle:
			verticesData = new Vertex[20];
			for (int i = 0; i < 20; i++)
			{
				angle += 2 * PI*i / 20;
				verticesData[numVertices].position.x = (float)(mWidth / 2 * cos(angle));
				verticesData[numVertices].position.y = (float)(mWidth / 2 * sin(angle));
				verticesData[numVertices++].position.z = z;
			}
			break;
		case b2Shape::e_polygon:
			verticesData = new Vertex[6];
			verticesData[0].position.x = x - width / 2;  verticesData[0].position.y = y + height / 2;  verticesData[0].position.z = z;
			verticesData[1].position.x = x - width / 2;  verticesData[1].position.y = y - height / 2;  verticesData[1].position.z = z;
			verticesData[2].position.x = x + width / 2;  verticesData[2].position.y = y - height / 2;  verticesData[2].position.z = z;

			verticesData[3].position.x = x - width / 2;  verticesData[3].position.y = y + height / 2;  verticesData[3].position.z = z;
			verticesData[4].position.x = x + width / 2;  verticesData[4].position.y = y + height / 2;  verticesData[4].position.z = z;
			verticesData[5].position.x = x + width / 2;  verticesData[5].position.y = y - height / 2;  verticesData[5].position.z = z;
			numVertices = 6;
			break;
		default:
			break;
		}
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = verticesData;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(verticesData), D3D11_BIND_VERTEX_BUFFER);
	if (m_vertexBuffer.Get() == NULL){
		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
			)
			);
	}
	else{

	}
	unsigned short indices[] =
	{
		0, 1, 2,
		3, 4, 5
	};

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(indices), D3D11_BIND_INDEX_BUFFER);
	int m_indexCount = ARRAYSIZE(indices);
	if (m_indexBuffer.Get() == NULL){
		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
			)
			);
	}
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_d3dContext->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	m_d3dContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,
		0
		);

	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	/*m_d3dContext->DrawIndexed(
	m_indexCount,
	0,
	0
	);*/

	/*m_vertexBuffer.Get()->Release();
	m_indexBuffer.Get()->Release();
	m_d3dContext->IASetVertexBuffers(
	0,
	1,
	m_vertexBuffer.GetAddressOf(),
	&stride,
	&offset
	);

	m_d3dContext->IASetIndexBuffer(
	m_indexBuffer.Get(),
	DXGI_FORMAT_R16_UINT,
	0
	);*/
}
#endif

Entity2D::Entity2D()
{
	mPosition=Vector3();
	mScaleFactor=Vector3(1.0f,1.0f,1.0f);
	mRotateAngles=Vector3();
	isFlying=false;
	isMoving=false;
	//directionFly=1;
	flytime=0.0f;
	velocity=0.0f;
	eslapedtimeFly=0.0f;
	targetMove=1;
	eslapedtimeMove=0.0f;
	mModel=0;
	isCollisionable=false;
	isMoving=false;
	isFlying=false;
	isMoveUp=false;
	isMoveDown=false;
	isMoveLinear=false;
	mMoveDirect=MoveRight;

	isActiveInGame=true;
	
}

Entity2D::~Entity2D()
{
	if (!mModel)
		return;
	Delete(mModel);
	mShader->UnRefenrence();
	if (mShader->GetReference()<=0)
		Delete(mShader);
}

Bound Entity2D::GetBound()
{
	mBound.bottomleft.x=mBound.topleft.x = mPosition.x - mBound.mWidth / 2;
	mBound.bottomright.x=mBound.topright.x = mPosition.x + mBound.mWidth / 2;
	mBound.topleft.y = mBound.topright.y = mPosition.y + mBound.mHeight / 2;
	mBound.bottomleft.y = mBound.bottomright.y = mPosition.y - mBound.mHeight / 2;

	float len = mBound.mWidth / 2;
	Vector2 orginTop = Vector2(mPosition.x, mBound.topleft.y);
	Vector2 orginBottom = Vector2(mPosition.x, mBound.bottomleft.y);
	mBound.topright.x = len*cos(mRotateAngles.z)+orginTop.x;
	mBound.topleft.x = orginTop.x-len*cos(mRotateAngles.z);
	mBound.topright.y = len*sin(mRotateAngles.z)+orginTop.y;
	mBound.topleft.y = orginTop.y - len*sin(mRotateAngles.z);

	mBound.bottomright.x = len*cos(mRotateAngles.z) + orginBottom.x;
	mBound.bottomleft.x = orginBottom.x - len*cos(mRotateAngles.z);
	mBound.bottomright.y = len*sin(mRotateAngles.z) + orginBottom.y;
	mBound.bottomleft.y = orginBottom.y - len*sin(mRotateAngles.z);

	Vector2 temp;
	if (mRotateAngles.z >= PI / 2){
		temp = mBound.topleft;
		mBound.topleft = mBound.topright;
		mBound.topright = temp;

		temp = mBound.bottomleft;
		mBound.bottomleft = mBound.bottomright;
		mBound.bottomright = temp;
	}

	if (mBound.topleft.x > mBound.topright.x){
		temp = mBound.topleft;
		mBound.topleft = mBound.topright;
		mBound.topright = temp;

		temp = mBound.bottomleft;
		mBound.bottomleft = mBound.bottomright;
		mBound.bottomright = temp;
	}

	if (mBound.topleft.y < mBound.bottomleft.y){
		temp = mBound.topleft;
		mBound.topleft = mBound.bottomleft;
		mBound.bottomleft = temp;

		temp = mBound.topright;
		mBound.topright = mBound.bottomright;
		mBound.bottomright = temp;
	}
	return mBound;
}


void Entity2D::GoUp(float v)
{
	/*if (isMoveUp)
		return;*/
	isFlying=true;
	isMoveUp=true;
	isMoveDown=false;
	flytime=v/trongtruong;
	
	y0=mPosition.y;
	velocity=v;
	mMoveDirect=MoveUp;
	eslapedtimeFly = 0.0f;
}
void Entity2D::GoDown(float v)
{
	/*if (isMoveDown)
		return;*/
	isFlying=true;
	isMoveDown=true;
	isMoveUp=false;
	flytime=v/trongtruong;
	
	y0=mPosition.y;
	velocity=v;
	mMoveDirect=MoveDown;
	eslapedtimeFly = 0.0f;
}
void Entity2D::FlipX()
{
	if (targetMove==1)
	{
		mRotateAngles.y=-PI;
		mMoveDirect=MoveLeft;
	}
	else
	{
		mRotateAngles.y=0;
		mMoveDirect=MoveRight;
	}
	targetMove=-targetMove;
}

void Entity2D::StopFlying()
{
	this->isMoveUp = false;
	this->isMoveDown = false;
}

bool Entity2D::IsPicked(Vector2 pickingPos,BaseCamera* cam)
{
	Vector3 pos=ConvertCoordinate2D3DByZIndex(cam,pickingPos,mPosition.z);
	

	mBound = GetBound();

	if (mBound.topleft.x <= pos.x && mBound.topright.x >= pos.x && mBound.topleft.y >= pos.y && mBound.bottomleft.y <= pos.y)
		return true;
	else if (mBound.topleft.x <= pos.x && mBound.topright.x >= pos.x&&
		mBound.bottomleft.x <= pos.x && mBound.bottomright.x >= pos.x && 
		(mBound.topright.y >= pos.y && mBound.bottomleft.y <= pos.y ||
		mBound.topleft.y >= pos.y && mBound.bottomright.y <= pos.y))
		return true;
	return false;
}

Vector3 Entity2D::GetPosition()
{
	Vector3 pos=mPosition;
	return pos;
}

Vector3 Entity2D::GetScaleFactor()
{
	return mScaleFactor;
}

Vector3 Entity2D::GetRotateAngles()
{
	return mRotateAngles;
}

string Entity2D::GetName(){
	return mName;
}

void Entity2D::SetPosition(Vector3 value)
{
	mPosition=value;
	mDepth = value.z / maxDepthOffset;
}

void Entity2D::SetScaleFactor(Vector3 value)
{
	mScaleFactor=value;
	mBound.mWidth = mWidth*value.x;
	mBound.mHeight = mHeight*value.y;
}

void Entity2D::SetRotateAngles(Vector3 value)
{
	mRotateAngles=value;
}

void Entity2D::SetShader(Shaders* sha)
{
	mShader=sha;
	sha->Refenrence();
}

void Entity2D::SetName(string value){
	mName = value;
}

Matrix Entity2D::GetWorldMatrix()
{
	Matrix World;
	World.SetIdentity();
	//World.SetScale(10.0f);
	Matrix Scale=Matrix().SetIdentity().SetScale(mScaleFactor);
	Matrix Translate=Translate.SetIdentity();
	Translate.SetTranslation(mPosition);
	Matrix RotationX,RotationY,RotationZ;
	RotationX.SetIdentity();
	RotationY.SetIdentity();
	RotationZ.SetIdentity();
	RotationX.SetRotationX(mRotateAngles.x);
	RotationY.SetRotationY(mRotateAngles.y);
	RotationZ.SetRotationZ(mRotateAngles.z);
	Matrix Rotation=RotationZ*RotationX*RotationY;
	return Scale*Rotation*Translate;
}

void Entity2D::update(float deltatime)
{
	if (!isActiveInGame)
		return;

	if (isMoveUp)
	{
		eslapedtimeFly+=deltatime*2.0f;
		mPosition.y=-0.5f*trongtruong*eslapedtimeFly*eslapedtimeFly + velocity*eslapedtimeFly +y0;
		float v=velocity+trongtruong*eslapedtimeFly;
		if (eslapedtimeFly>=flytime)
		{
			mMoveDirect=MoveDown;
		}
	}
	else if (isMoveDown)
	{
		eslapedtimeFly+=deltatime;
		mPosition.y=0.5f*trongtruong*eslapedtimeFly*eslapedtimeFly + velocity*eslapedtimeFly +y0;
	}
	if (isMoving && !isMoveLinear)
	{
		eslapedtimeMove+=deltatime;
		//mPosition.x=targetMove*movingSpeed*eslapedtimeMove +x0;
		mPosition.x= targetMove*movingSpeed*eslapedtimeMove + -targetMove*0.5f*giatoc*eslapedtimeMove*eslapedtimeMove +x0;
		float v=targetMove*movingSpeed+-targetMove*giatoc*eslapedtimeMove;
		if ((v<=0.0f && targetMove==1) || (v>=0.0f && targetMove==-1))
		{
			isMoving=false;
			eslapedtimeMove=0;
		}

	}
}

void Entity2D::SetMoveDirect(MoveDirect moveDir)
{
	mMoveDirect=moveDir;
}

void Entity2D::MoveForward(float speed)
{
	/*if (targetMove==-1)
		FlipX();*/
	//
	movingOffset=2.0f;
	movingSpeed=speed;
	movetime=movingOffset/speed;
	isMoving=true;
	isMoveLinear=false;
	x0=mPosition.x;
	eslapedtimeMove=0;
}

void Entity2D::MoveLinearUp(float offset)
{
	mPosition.y+=offset;
	isMoveLinear=true;
	isMoving=true;
	mMoveDirect=MoveUp;
}

void Entity2D::MoveLinearDown(float offset)
{
	mPosition.y-=offset;
	isMoveLinear=true;
	isMoving=true;
	mMoveDirect=MoveDown;
}

void Entity2D::MoveLinearForward(float offset)
{
	/*if (targetMove==-1)
		FlipX();*/
	mPosition.x+=offset;
	isMoveLinear=true;
	isMoving=true;
	mMoveDirect=MoveRight;
}

void Entity2D::MoveLinearBackward(float offset)
{
	/*if (targetMove==1)
		FlipX();*/
	mPosition.x-=offset;
	isMoveLinear=true;
	isMoving=true;
	mMoveDirect=MoveLeft;
}

void Entity2D::MoveBackward(float speed)
{
	/*if (targetMove==1)
		FlipX();*/
	movingOffset=0.5f;
	movingSpeed=speed;
	movetime=movingOffset/speed;
	isMoving=true;
	isMoveLinear=false;
	x0=mPosition.x;
	eslapedtimeMove=0;
}

vector<Entity2D*> Entity2D::entityAround(float nearX,float nearY,vector<Entity2D*> totalEntity)
{
	float xmin = mPosition.x - nearX;
	float xmax = mPosition.x + nearX;
	float ymin = mPosition.y - nearY;
	float ymax = mPosition.y + nearY;
	Bound mRect = GetBound();
	vector<Entity2D*> res;
	res.clear();
	int n = totalEntity.size();
	if (nearX == 0)
	{
		if (mMoveDirect == MoveUp)
		{
			ymin = mPosition.y;
		}
		else if (mMoveDirect == MoveDown)
		{
			ymin = mPosition.y - 0.5f;
			ymax = mPosition.y;
		}
		for (int i = 0; i<n; i++)
		{
			Vector3 pos = totalEntity[i]->mPosition;
			if (
				totalEntity[i]->mPosition.y >= ymin && totalEntity[i]->mPosition.y <= ymax &&
				totalEntity[i] != this&&
				totalEntity[i]->IsActiveInGame() &&
				pos.x >= mPosition.x && pos.x<mPosition.x + mWidth)
			{
				res.push_back(totalEntity[i]);
			}
		}
	}
	else if (nearY == 0)
	{
		if (mMoveDirect == MoveRight)
		{
			xmin = mPosition.x;
		}
		else if (mMoveDirect == MoveLeft)
		{
			xmin = mPosition.x - 0.5f;
			xmax = mPosition.x;
		}
		for (int i = 0; i<n; i++)
		{
			Vector3 pos = totalEntity[i]->mPosition;
			if (totalEntity[i]->mPosition.x >= xmin && totalEntity[i]->mPosition.x <= xmax &&
				totalEntity[i] != this&&
				totalEntity[i]->IsActiveInGame() &&
				pos.y >= mPosition.y && pos.y<mPosition.y + mHeight)
			{
				res.push_back(totalEntity[i]);
			}
		}
	}
	else
	{
		for (int i = 0; i<n; i++)
		{
			Vector3 pos = totalEntity[i]->mPosition;
			if (totalEntity[i]->mPosition.x >= xmin && totalEntity[i]->mPosition.x <= xmax &&
				totalEntity[i]->mPosition.y >= ymin && totalEntity[i]->mPosition.y <= ymax &&
				totalEntity[i] != this&&
				totalEntity[i]->IsActiveInGame())
			{
				res.push_back(totalEntity[i]);
			}
		}
	}
	return res;
}

CollisDirection Entity2D::CollisWith(Entity2D* ent)
{
	if (!this->isCollisionable || !ent->isCollisionable)
		return CollisNone;
	if ( this==ent)
		return CollisNone;
	

	Bound rec1=GetBound();
	Bound rec2=ent->GetBound();
	float xmin=rec2.topleft.x;
	float xmax=rec2.topright.x;
	float ymin=rec2.bottomleft.y;
	float ymax=rec2.topleft.y;

	bool sumWidthCondition=false;
	bool sumHeightCondition=false;

	if (ent->GetPosition().y>mPosition.y)
	{
		sumHeightCondition=(ymax-rec1.bottomleft.y<=rec1.mHeight+rec2.mHeight);
	}
	else
	{
		sumHeightCondition=(rec1.topleft.y-ymin<=rec1.mHeight+rec2.mHeight);
	}

	if (ent->GetPosition().x>mPosition.x)
	{
		sumWidthCondition=(xmax-rec1.topleft.x<=rec1.mWidth+rec2.mWidth);
	}
	else
	{
		sumWidthCondition=(rec1.topright.x-xmin<=rec1.mWidth+rec2.mWidth);
	}


	bool collised= ((rec1.topleft.x<=xmax) && (xmin<=rec1.topright.x)&&
		(rec1.topleft.y>=ymin) && (ymax>=rec1.bottomleft.y)) && sumWidthCondition && sumHeightCondition;

	if (collised)
	{
		
		if (rec1.topleft.y>=rec2.topleft.y && rec1.bottomleft.y<=rec2.topleft.y)
			return CollisBottom;
		if (rec1.topleft.y>=rec2.bottomleft.y && rec1.bottomleft.y<=rec2.bottomleft.y)
 			return CollisTop;
		if (rec1.topright.x>=rec2.topleft.x)
			return CollisRight;
		if (rec1.topleft.x<=rec2.topright.x)
			return CollisLeft;
	}
	
	return CollisNone;
}

void Entity2D::drawBound()
{
#if defined Win32
	glUseProgram(Global::BoundShader->program);
#endif
#if defined WindowPhone
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
	Translate.SetTranslation(mPosition);
	Matrix RotationX, RotationY, RotationZ;
	RotationX.SetIdentity();
	RotationY.SetIdentity();
	RotationZ.SetIdentity();
	RotationX.SetRotationX(mRotateAngles.x);
	RotationY.SetRotationY(mRotateAngles.y);
	RotationZ.SetRotationZ(mRotateAngles.z);
	Matrix Rotation = RotationZ*RotationX*RotationY;
	World= Rotation*Translate;
	MVP = World*Global::currentCamera->ViewMatrix() * Global::currentCamera->projection;
#if defined Win32 || defined Android
	if (Global::BoundShader->u_MatrixMVP != -1)	// Send MVP
		glUniformMatrix4fv(Global::BoundShader->u_MatrixMVP, 1, GL_FALSE, (float*)&MVP);
#endif
#if defined WindowPhone
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
#endif
	GetBound().Draw(mPosition.z);
}


Matrix Entity2D::GetMVP()
{
	Matrix MVP, world;
	MVP.SetIdentity();
	world.SetIdentity();
	world = GetWorldMatrix();
	MVP = world*Global::currentCamera->ViewMatrix() * Global::currentCamera->projection;
	return MVP;
}

