#include "EntityFactory.h"
#include "AnimatedSprite.h"
#include "AnimatedSpritePhysic.h"
#include "Utils.h"


map<string,Entity2D*> EntityFactory::mRegisteredEntityMap=map<string,Entity2D*>();
bool EntityFactory::isInit=false;
Entity2D* EntityFactory::RootSprite = new AnimatedSprite();

void EntityFactory::InitEntity()
{
	if (isInit)
		return;
	RegisterEntity("Sprite",new Sprite());
	RegisterEntity("AnimatedSprite", new AnimatedSprite());
	RegisterEntity("AnimatedSpritePhysic", new AnimatedSpritePhysics());
	isInit=true;
}


void EntityFactory::RegisterRootSprite(Entity2D* type){
	if (type != 0 && type != NULL && type != nullptr){
		Delete(RootSprite);
		RootSprite = type;
	}
}
void EntityFactory::RegisterEntity(string name,Entity2D* ent)
{
	if (mRegisteredEntityMap.count(name)<1)
		mRegisteredEntityMap[name]=ent;
}

void EntityFactory::RegisterEntity(char* name,Entity2D* ent)
{
	if (mRegisteredEntityMap.count(string(name))<1)
		mRegisteredEntityMap[string(name)]=ent;
}

Entity2D* EntityFactory::CreateAEntity(char *name)
{
	string sName=string(name);
	if (mRegisteredEntityMap.count(sName) < 1){
		mRegisteredEntityMap[sName] = RootSprite->Create();
	}
	Entity2D* temp=mRegisteredEntityMap[sName];
	return temp->Create();
}

Entity2D* EntityFactory::CreateAEntity(string name)
{
	string sName = string(name);
	if (mRegisteredEntityMap.count(sName) < 1){
		mRegisteredEntityMap[sName] = RootSprite->Create();
	}
	Entity2D* temp = mRegisteredEntityMap[sName];
	return temp->Create();
}

void EntityFactory::Close()
{
	for (auto&x:mRegisteredEntityMap)
	{
		Delete(x.second);
	}
	Delete(RootSprite);
}