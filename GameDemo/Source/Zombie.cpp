#include "pch.h"
#include <stdio.h>
#include "Zombie.h"

Zombie::Zombie(){
	this->zombieCollision = false;
	this->hP = 100;
	this->strenght = 10;
	this->isZombieFight = false;
}

void Zombie::Fight(bool isCollision){
	zombieCollision = isCollision;
}

void Zombie::Hurt(int damage){
	hP -= damage;
	MoveLinearForward(0.1f);
	if (hP < 1)
		isActiveInGame = false;
}

void Zombie::UpdateFighting(float deltaTime){
	/*if (heroCollision){
		if (!isHeroFight){
			SetUpFrame(1, 0, 4);
			isHeroFight = true;
		}
	}
	else{
		if (GetCurrentFrame() > 3){
			SetUpFrame(0, 0, 0);
			isHeroFight = false;
		}
	}*/
}
Zombie* Zombie::Create(){
	return new Zombie();
}
Zombie* Zombie::Clone(){
	Zombie* zombie = new Zombie();
	CloneAttribute(zombie);
	zombie->zombieCollision = zombieCollision;
	zombie->hP = hP;
	zombie->strenght = strenght;
	zombie->isZombieFight = isZombieFight;

	return zombie;
}
