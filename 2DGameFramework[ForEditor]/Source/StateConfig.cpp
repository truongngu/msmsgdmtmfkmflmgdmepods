#include "StateConfig.h"
#include "Utils.h"

EntityInfor::~EntityInfor(){
	for (auto&x : attributeList)
	{
		Delete(x.second);
	}
}

void EntityInfor::addFrame(std::string frameName, vector<int> dirFrame){
	registerFrame[frameName] = dirFrame;
	registerFrameName.push_back(frameName);
}

void* EntityInfor::getAttribute(std::string attrName){
	return attributeList[attrName];
}

void EntityInfor::setAttribute(std::string attrName, void* attrValue){
	attributeList[attrName] = attrValue;
}

StateConfig::StateConfig(){
	inforList = vector<EntityInfor*>();
}

StateConfig::~StateConfig(){
	for (auto&x : inforList)
	{
		Delete(x);
	}
}

void StateConfig::ReadConfig(const char* path2Config){
	inforList.clear();
	//const char* path2Config = this->path2Config.c_str();
	FILE* pFile = fopen(path2Config, "r");
	if (pFile == NULL)
		return;
	char buffer[MAX_LEN];

	int numEntity;
	int iCount = 0;
	//Pass Texure
	ReadLine(pFile, buffer);
	while (true)
	{
		ReadLine(pFile, buffer);
		if (strstr(buffer, "//End"))
			break;
		EntityInfor* infor = new EntityInfor();

		//read texture path
		char path[MAX_LEN];
		int t = ScanPath(buffer, path, 0);
		strcpy(infor->texturePath, path);
		//read className
		ReadLine(pFile, buffer);
		strcpy(infor->className, buffer);

		//Frame Sprite
		ReadLine(pFile, buffer);
		int iPos = 0;
		float duration = 0.0f, width = 0.0f, height = 0.0f;
		int numSpriteX = 0, numSpriteY = 0, numSprite = 0;
		iPos = ScanNumberFloat32(buffer, &infor->duration, iPos);
		iPos = ScanNumberFloat32(buffer, &infor->width, iPos);
		iPos = ScanNumberFloat32(buffer, &infor->height, iPos);
		iPos = ScanNumberInt32(buffer, &infor->numSpriteX, iPos);
		iPos = ScanNumberInt32(buffer, &infor->numSpriteY, iPos);
		iPos = ScanNumberInt32(buffer, &infor->numSprite, iPos);

		//read shader path
		//vertext shader
		ReadLine(pFile, buffer);
		iPos = ScanPath(buffer, path, 0);
		strcpy(infor->vShaderPath, path);
		//pixel shader
		ReadLine(pFile, buffer);
		iPos = ScanPath(buffer, path, 0);
		strcpy(infor->fShaderPath, path);
		//read number of frame //animated sprite
		ReadLine(pFile, buffer);
		int numFrame;
		ScanNumberInt32(buffer, &numFrame, 0);
		for (int i = 0; i < numFrame; i++)
		{
			//name of frame
			char nFrame[255];
			ReadLine(pFile, nFrame);
			//vector frame
			//number of frame
			ReadLine(pFile, buffer);
			int numAFrame;
			iPos = ScanNumberInt32(buffer, &numAFrame, 0);
			ReadLine(pFile, buffer);
			int posFrame;
			iPos = 0;
			vector<int> frameContent = vector<int>();
			for (int i = 0; i < numAFrame; i++){
				iPos = ScanNumberInt32(buffer, &posFrame, iPos);
				frameContent.push_back(posFrame);
			}

			infor->addFrame(nFrame, frameContent);
		}

		//Read number clone of sprite
		ReadLine(pFile, buffer);
		int numClone;
		ScanNumberInt32(buffer, &numClone, 0);
		infor->numClone = numClone;

		for (int i = 0; i<numClone; i++)
		{
			//read name of clone
			ReadLine(pFile, buffer);
			std::string nameofClone = buffer;
			ReadLine(pFile, buffer);
			float x, y, z, flip;
			iPos = ScanNumberFloat32(buffer, &x, 0);
			iPos = ScanNumberFloat32(buffer, &y, iPos);
			iPos = ScanNumberFloat32(buffer, &z, iPos);
			iPos = ScanNumberFloat32(buffer, &flip, iPos);

			infor->positionClone.push_back(PositionClone(nameofClone, x, y, z, flip));
		}

		inforList.push_back(infor);
		iCount++;
	}
}
vector<EntityInfor*> StateConfig::getConfig(){
	return this->inforList;
}