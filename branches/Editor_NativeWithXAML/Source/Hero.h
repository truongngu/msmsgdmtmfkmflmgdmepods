#pragma once
#include "2DGameFramework\Game.h"
#include "2DGameFramework\Sprite.h"

class Hero:public Sprite
{
private:
	bool isHeroFight;
	bool heroCollision;
	int hP;
	int strenght;
public:
	Hero();
	void Fight(bool);
	Hero* Create();
	Hero* Clone();
	void UpdateFighting(float deltaTime);
	int getStrenght();
};

