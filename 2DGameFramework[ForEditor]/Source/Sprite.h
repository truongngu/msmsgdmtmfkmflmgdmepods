#pragma once
#include "../Utilities/utilities.h"
#include "Vertex.h"
#include "Shader.h"
#include "Entity2D.h"
#include "Texture.h"

enum SpriteState
{
	STOPPED = 0,
	PLAYING = 1
};

class Sprite : public Entity2D
{
protected:
	int mNumFrames;
	float mFramesPerSec;
	int mNumSpriteX, mNumSpriteY, mNumSprites;
	SpriteState mState;
	GLint uTexture, aTexCoord, uDx, uDy, uPercent;
	float m_percent;
	Vector3 m_Color;
	GLfloat m_Tran;
	bool m_IsProgress;
	bool isHUD;
	float eslapedTime;
	float dx,dy;
protected:
	int iCurrentFrame;
	int lastFrame;
	int beginFrame;

	bool isShowBound;
	void CloneAttribute(Sprite*);
public:
	Sprite(void);
	void SetIsHUD(){isHUD=true;}
	void SetShowBoundEnable(bool value){isShowBound=value;}
	void SetFramesPerSec(int value){mFramesPerSec=value;}
	void SetCurrentFrame(int value){iCurrentFrame=value;}
	void SetUpFrame(int currentFrame,int begin, int last){iCurrentFrame=currentFrame; beginFrame=begin; lastFrame=last;}
	int GetCurrentFrame(){return iCurrentFrame;}
	void Play() { mState = SpriteState::PLAYING; }
	void Stop() { mState = SpriteState::STOPPED; }
	virtual void Init();
	int Init(char* texture_path, float duration, float width, float height, 
			 int num_sprites_X, int num_sprites_Y, int num_sprites,float x = 0, float y = 0, float z = 0);
	int Init(Texture* texture, float FPS, float width, float height, 
			 int num_sprites_X, int num_sprites_Y, int num_sprites,float x = 0, float y = 0, float z = 0);
	virtual void draw();
	virtual void update(float deltaTime);
	virtual Sprite* Create();
	virtual Sprite* Clone();
	virtual ~Sprite(void);
};
