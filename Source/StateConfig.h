#pragma once
#include "Sprite.h"
#include <map>

struct PositionClone{
	PositionClone(){

	}
	PositionClone(float tx, float ty, float tz, float tflip = 0.0f){
		x = tx;
		y = ty;
		z = tz;
		flip = tflip;
		//get handler
	}
	PositionClone(std::string tnameOfClone, float tx, float ty, float tz, float tflip = 0.0f){
		x = tx;
		y = ty;
		z = tz;
		flip = tflip;
		nameOfClone = tnameOfClone;
	}

	float x, y, z, flip;
	std::string nameOfClone;
};

class EntityInfor{
public:
	char texturePath[255];
	char className[255];
	float duration;
	float width;
	float height;
	int numSpriteX;
	int numSpriteY;
	int numSprite;
	map<std::string, vector<int>> registerFrame;
	vector<std::string> registerFrameName;
	vector<PositionClone> positionClone;
	int numClone;
	char vShaderPath[255];
	char fShaderPath[255];
	map<std::string, void*> attributeList;
	~EntityInfor();
	EntityInfor(){
		positionClone = vector<PositionClone>();
		registerFrameName = vector<std::string>();
	}

	virtual void addFrame(std::string frameName, vector<int> dirFrame);

	virtual void* getAttribute(std::string attrName);

	virtual void setAttribute(std::string attrName, void* attrValue);
};

class StateConfig{
protected:
	//doc config len thanh mot list cac infor
	vector<EntityInfor*> inforList;
public:
	StateConfig();
	~StateConfig();
	virtual void ReadConfig(const char*);
	virtual void ReadTextConfig(const char*);
	virtual void ReadXMLConfig(const char*);
	vector<EntityInfor*> getConfig();
};
