#include "Sprite.h"
#include "RectangleMesh.h"
#include "Global.h"

Sprite::Sprite(void) : Entity2D(),
m_Color(0.0f, 0.0f, 0.0f), m_Tran(1.0f), m_IsProgress(false)
{
	m_percent = 1.0f;
	mState = SpriteState::PLAYING;
	mPosition = Vector3(0.0f, 0.0f, 0.0f);
	mScaleFactor = Vector3(1.0f, 1.0f, 1.0f);
	mRotateAngles = Vector3(0.0f, 0.0f, 0.0f);
	isHUD = false;
	eslapedTime = 0.0f;
	isShowBound = false;
}

Sprite::~Sprite(void)
{
	//Entity2D::~Entity2D();
	//Delete(mModel);
}

//int Sprite::Init(Resource* texture, float duration, float width, float height, int num_sprites_X, int num_sprites_Y, int num_sprites,float x = 0, float y = 0, float z = 0)
//{
//	totalTime = 0;
//	this->duration = duration;
//	this->numSprites = num_sprites;
//	this->numSpritesX = num_sprites_X;
//	this->numSpritesY = num_sprites_Y;
//
//	m_Textures.push_back(static_cast<Texture*>(texture));
//
//	//triangle data (heap)
//	Vertex verticesData[6];
//
//	verticesData[0].position.x = x - width / 2;  verticesData[0].position.y = y + height / 2;  verticesData[0].position.z = z;
//	verticesData[1].position.x = x - width / 2;  verticesData[1].position.y = y -height / 2;  verticesData[1].position.z = z;
//	verticesData[2].position.x = x + width / 2;  verticesData[2].position.y = y -height / 2;  verticesData[2].position.z = z;
//
//	verticesData[3].position.x = x - width / 2;  verticesData[3].position.y = y + height / 2;  verticesData[3].position.z = z;
//	verticesData[4].position.x = x + width / 2;  verticesData[4].position.y = y + height / 2;  verticesData[4].position.z = z;
//	verticesData[5].position.x = x + width / 2;  verticesData[5].position.y = y - height / 2;  verticesData[5].position.z = z;
//	// test uv
//	float dx = float(1) / num_sprites_X;
//	float dy = float(1) / num_sprites_Y;
//	int i = 0; int j = 0;
//	verticesData[0].texcoord.x = i * dx;			verticesData[0].texcoord.y = 1 - j * dy;
//	verticesData[1].texcoord.x = i * dx;			verticesData[1].texcoord.y = 1 - j * dy - dy;
//	verticesData[2].texcoord.x = i * dx + dx;		verticesData[2].texcoord.y = 1 - j * dy - dy;
//
//	verticesData[3].texcoord.x = i * dx;			verticesData[3].texcoord.y = 1 - j * dy;
//	verticesData[4].texcoord.x = i * dx + dx;		verticesData[4].texcoord.y = 1 - j * dy;
//	verticesData[5].texcoord.x = i * dx + dx;		verticesData[5].texcoord.y = 1 - j * dy - dy;
//
//
//	//buffer object
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	//creation of shaders and program 
//	myShaders.Init("sdcard/Resources/Shaders/SpriteShaderVS.vsh", "sdcard/Resources/Shaders/SpriteShaderFS.fsh");
//	aTexCoord = glGetAttribLocation(myShaders.program, "a_TexCoord");
//	uTexture = glGetUniformLocation(myShaders.program, "u_texture");
//	uPercent = glGetUniformLocation(myShaders.program, "u_percent");
//	//
//	uDx = glGetUniformLocation(myShaders.program, "u_dx");
//	uDy = glGetUniformLocation(myShaders.program, "u_dy");
//
//	m_MVPenabled = true;
//
//	return 0;
//}


int Sprite::Init(Texture* texture, float FPS, float width, float height, int num_sprites_X, int num_sprites_Y, int num_sprites, float x, float y, float z)
{
	//mNumFrames = 0;
	this->mFramesPerSec = FPS;
	this->mNumSprites = num_sprites;
	this->mNumSpriteX = num_sprites_X;
	this->mNumSpriteY = num_sprites_Y;
	iCurrentFrame = 0;
	beginFrame = 0;
	lastFrame = mNumSprites;
	mWidth = width;
	mHeight = height;
	//mScaleFactor.x=width;
	//mScaleFactor.y=height;
	mBound.mWidth = width;
	mBound.mHeight = height;
	mBound.mShape = 0;
	RectangleMesh* rec = new RectangleMesh(width, height, num_sprites_X, num_sprites_Y);
	this->mModel = new Model2D(rec, texture);
	Init();
	return 0;
}

void Sprite::Init()
{

}

void Sprite::draw()
{
	if (!isActiveInGame)
		return;
	/*if (mState == SpriteState::STOPPED)
		return;*/

#if defined Win32 || defined Android
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(mShader->program);
#endif
#if defined WindowPhone

	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);

	m_d3dContext->IASetInputLayout(mShader->m_inputLayout.Get());

	m_d3dContext->VSSetShader(
		mShader->vertexShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->PSSetShader(
		mShader->pixelShader.Get(),
		nullptr,
		0
		);
#endif
#if defined Win32 || defined Android
	if (mShader->u_Color != -1)
	{
		glUniform3f(mShader->u_Color, m_Color.x, m_Color.y, m_Color.z);
	}
	if (mShader->u_Tran != -1)
	{
		glUniform1f(mShader->u_Tran, m_Tran);
	}
	if (mShader->u_dx!=-1)
	{
		glUniform1f(mShader->u_dx, dx);
	}
	if (mShader->u_dy!=-1)
	{
		glUniform1f(mShader->u_dy, dy);
	}
#endif
	Matrix MVP, world;
	MVP.SetIdentity();
	world.SetIdentity();

	if (!isHUD)
	{
		world = GetWorldMatrix();
		MVP = world*Global::currentCamera->ViewMatrix() * Global::currentCamera->projection;
#if defined Win32 || defined Android
		if (mShader->u_MatrixMVP != -1)	// Send MVP
			glUniformMatrix4fv(mShader->u_MatrixMVP, 1, GL_FALSE, (float*)&MVP);
#endif
#if defined WindowPhone
		//world.Transpose();
		Matrix view = Global::currentCamera->ViewMatrix();

		MVP = world*view* Global::currentCamera->projection;

		MVPConstantBuffer m_constantBufferData;
		m_constantBufferData.MVP = MVP.Transpose();
		m_constantBufferData.World = world;
		m_constantBufferData.u_dxy = Vector4(dx, dy, 0, 0);

		if (m_constantBuffer.Get() == NULL){
			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				Global::m_d3ddevice->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
				);
		}

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
	}
#if defined Win32 || defined Android
	else if (mShader->u_MatrixMVP != -1)
	{
		//mScaleFactor=Vector3(100.0,100.0,10.0f);
		world = GetWorldMatrix();
		MVP = world;
		//MVP.SetIdentity();
		glUniformMatrix4fv(mShader->u_MatrixMVP, 1, GL_FALSE, (float*)&MVP);
#endif
#if defined WindowPhone
	else
	{
		
		MVPConstantBuffer m_constantBufferData;
		float tempz = mPosition.z;
		float tempy = mPosition.y;
		float tempx = mPosition.x;
		//static float ttz = 0;
		//mPosition.y = mPosition.x = ttz;
		//mPosition.z = 1;
		Matrix view = Global::currentCamera->ViewMatrix();
		world = GetWorldMatrix();
		MVP = world*view* Global::currentCamera->projection;

		m_constantBufferData.MVP = MVP.Transpose();
		mPosition.z = tempz;
		mPosition.x = tempx;
		mPosition.y = tempy;

		m_constantBufferData.u_dxy = Vector4(dx, dy, 0, 0);
		if (m_constantBuffer.Get() == NULL){
			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				Global::m_d3ddevice->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
				);
		}

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
	}
	mModel->Draw(mShader);
	if (isShowBound)
		drawBound();
}

void Sprite::update(float deltaTime)
{
	if (mState == SpriteState::STOPPED)
	{
		iCurrentFrame = 0;
		//return;
	}
	else
	{
		eslapedTime += deltaTime;
		if (eslapedTime > 1.0f / mFramesPerSec)
		{
			eslapedTime = 0;
			iCurrentFrame++;
			if (iCurrentFrame > lastFrame)
				iCurrentFrame = beginFrame;
		}
	}
	int ix = iCurrentFrame % mNumSpriteX;
	int iy = iCurrentFrame / mNumSpriteX;

	// Set texture Coordinates displacement
	dx = float(ix) / float(mNumSpriteX);
	dy = float(iy) / float(mNumSpriteY);
	Entity2D::update(deltaTime);
}


Sprite* Sprite::Create()
{
	return new Sprite();
}

void Sprite::CloneAttribute(Sprite* ins)
{
	ins->mModel = new Model2D(this->mModel->GetMesh(), this->mModel->GetTexture());
	ins->mNumFrames = this->mNumFrames;
	ins->mNumSprites = this->mNumSprites;
	ins->mNumSpriteX = this->mNumSpriteX;
	ins->mNumSpriteY = this->mNumSpriteY;
	ins->mWidth = this->mWidth;
	ins->mHeight = this->mHeight;
	ins->mFramesPerSec = this->mFramesPerSec;
	ins->beginFrame = this->beginFrame;
	ins->lastFrame = this->lastFrame;
	ins->SetShader(this->mShader);
	ins->iCurrentFrame = beginFrame;
	ins->isCollisionable = this->isCollisionable;
	ins->mBound.mWidth = this->mBound.mWidth;
	ins->mBound.mHeight = this->mBound.mHeight;
	ins->mBound.mShape = 0;
}

Sprite* Sprite::Clone()
{
	Sprite* newSprite = new Sprite();
	newSprite->mModel = new Model2D(this->mModel->GetMesh(), this->mModel->GetTexture());
	newSprite->mNumFrames = this->mNumFrames;
	newSprite->mNumSprites = this->mNumSprites;
	newSprite->mNumSpriteX = this->mNumSpriteX;
	newSprite->mNumSpriteY = this->mNumSpriteY;
	newSprite->mWidth = this->mWidth;
	newSprite->mHeight = this->mHeight;
	newSprite->mFramesPerSec = this->mFramesPerSec;
	newSprite->beginFrame = this->beginFrame;
	newSprite->lastFrame = this->lastFrame;
	newSprite->SetShader(this->mShader);
	newSprite->iCurrentFrame = beginFrame;
	newSprite->isCollisionable = this->isCollisionable;
	newSprite->mBound.mWidth = this->mBound.mWidth;
	newSprite->mBound.mHeight = this->mBound.mHeight;
	newSprite->mBound.mShape = 0;
	//this->mModel->GetMesh()->Refenrence();
	return newSprite;
}
