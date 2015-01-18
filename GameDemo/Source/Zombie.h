	#pragma once
#include "2DGameFramework\Game.h"
#include "2DGameFramework\Sprite.h"

class Zombie:public Sprite
{
private:
	bool isZombieFight;
	bool zombieCollision;
	int hP;
	int strenght;
public:
	Zombie();
	
	void Fight(bool);
	void Hurt(int damege);
	Zombie* Create();
	Zombie* Clone();
	void UpdateFighting(float deltaTime);
};

