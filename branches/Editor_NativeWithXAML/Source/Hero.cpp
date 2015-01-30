#include "pch.h"
#include <stdio.h>
#include "Hero.h"

Hero::Hero(){
	this->heroCollision = false;
	this->hP = 100;
	this->strenght = 10;
	this->isHeroFight = false;
}

void Hero::Fight(bool isCollision){
	heroCollision = isCollision;
}

void Hero::UpdateFighting(float deltaTime){
	if (heroCollision){
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
	}
}

Hero* Hero::Create(){
	return new Hero();
}
Hero* Hero::Clone(){
	Hero* hero = new Hero();
	CloneAttribute(hero);
	hero->heroCollision = heroCollision;
	hero->hP = hP;
	hero->strenght = strenght;
	hero->isHeroFight = isHeroFight;
	
	return hero;
}

int Hero::getStrenght(){
	return strenght;
}