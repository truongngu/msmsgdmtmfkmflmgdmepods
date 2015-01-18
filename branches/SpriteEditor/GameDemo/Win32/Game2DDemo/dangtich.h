#pragma once
#include "2DGameFramework\Sprite.h"
#include "2DGameFramework\State.h"
#include "Bird.h"


class Gamemoi:public State
{
private:
	Bird* bird;
	Sprite* back;
	Sprite* ground;
	Sprite* pipe;
	Sprite* ground2;
	bool birdIsFly;
	float y0;
	float eslapedTime;
	float rotateOffset;
	bool hitted;
	vector<Sprite*> pipelist;
	int pipeIndex;
	int maxX;
	bool bPastPipe;
	int score;
	bool isGameOver;
	float offsetX;
	int countX;
	float offsetXPipe;
protected:
public:
	Gamemoi();
	~Gamemoi();
	void Init();
	void Mouse(MouseData mouse, bool bIsDown);
	void Key(unsigned char key, bool bIsPressed);
	void onTouch(int id,float x,float y);
	void Update(float deltatime);
	void ResumeSound();
	void InitPipe();
	void CreatePipe();	
	void Draw();
	void RotateBird(float angle);
	bool CheckCollision(Bound, Bound);
};