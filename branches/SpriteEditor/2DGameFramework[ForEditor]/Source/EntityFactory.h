#pragma once
#include <map>
#include <string>
#include "Entity2D.h"
using namespace std;

class EntityFactory
{
private:
	static map<std::string,Entity2D*> mRegisteredEntityMap;
	static bool isInit;
	static Entity2D* RootSprite;
public:
	static void InitEntity();
	static void RegisterRootSprite(Entity2D*);
	static void RegisterEntity(string, Entity2D* entity);
	static void RegisterEntity(char*, Entity2D* entity);
	static Entity2D* CreateAEntity(char* name);
	static Entity2D* CreateAEntity(string name);
	static void Close();
};