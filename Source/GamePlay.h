#pragma once
#include "2DGameFramework\Sprite.h"
#include "2DGameFramework\State.h"

class GamePlayState:public State
{
private:
	Sprite* back;
	float elapseTime;
protected:
public:
	GamePlayState();
	~GamePlayState();
	void Init();
	void Mouse(MouseData mouse, bool bIsDown);
	void Key(unsigned char key, bool bIsPressed);
	void onTouch(int id,float x,float y);
	void Update(float deltatime);
	void ResumeSound();
	void Draw();
};