#include "StateConfig.h"
#include "Utils.h"
#include "ConvertUtils.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

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
	if (strstr(path2Config, ".txt"))
	{
		ReadTextConfig(path2Config);
	}
	else if (strstr(path2Config, ".xml")){
		ReadXMLConfig(path2Config);
	}
	else{
		Log("Not Support Config File Type");
		throw("Not Support Config File Type");
	}
}

void StateConfig::ReadTextConfig(const char* path2Config){
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

void StateConfig::ReadXMLConfig(const char* path2Config){
	inforList.clear();
	rapidxml::file<> xmlFile(path2Config); // Default template is char
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* cur_node = doc.first_node("CONFIG")->first_node("ENTITY_LIST");
	if (!cur_node){
		Log("Error in config file format!");
		throw ("Error in config file format!");
	}
	xml_node<>* entity_node = 0;
	entity_node = cur_node->first_node("ENTITY");
	do{
		if (entity_node != 0){
			EntityInfor* infor = new EntityInfor();
			xml_node<>* texture_node = entity_node->first_node("TEXTURE_PATH");
			xml_node<>* class_node = entity_node->first_node("CLASS_NAME");
			xml_node<>* sprite_node = entity_node->first_node("SPRITE_ATTRIBUTE");
			xml_node<>* shader_node = entity_node->first_node("SHADER");
			xml_node<>* frame_node = entity_node->first_node("FRAME_INFO");
			xml_node<>* clone_node = entity_node->first_node("CLONE_INFO");
			
			//Set Texture Path
			strcpy(infor->texturePath, texture_node->value());
			//Set Class Name
			strcpy(infor->className, class_node->value());
			//Set Sprite Attribute
			infor->duration=atof(sprite_node->first_attribute("fps")->value());
			infor->width = atof(sprite_node->first_attribute("width")->value());
			infor->height = atof(sprite_node->first_attribute("height")->value());
			infor->numSpriteX = atof(sprite_node->first_attribute("num_spriteX")->value());
			infor->numSpriteY = atof(sprite_node->first_attribute("num_spriteY")->value());
			infor->numSprite = atof(sprite_node->first_attribute("total_sprite")->value());
			//Set Shader Path
			strcpy(infor->vShaderPath, shader_node->first_attribute("vertex")->value());
			strcpy(infor->fShaderPath, shader_node->first_attribute("fragment")->value());
			
			//Set Frame List
			int numFrameAnimation = atoi(frame_node->first_attribute("num_frame")->value());
			xml_node<>* frame_ChildNode = frame_node->first_node("FRAME");
			while (frame_ChildNode != 0 && numFrameAnimation>0){
				int posFrame;
				int iPos = 0;
				vector<int> frameContent = vector<int>();
				for (int i = 0; i < numFrameAnimation; i++){
					iPos = ScanNumberInt32(frame_ChildNode->value(), &posFrame, iPos);
					frameContent.push_back(posFrame);
				}
				infor->addFrame(string(frame_ChildNode->first_attribute("name")->value()), frameContent);
				frame_ChildNode=frame_ChildNode->next_sibling("FRAME");
			}
			//Set Clone List
			int numClone = atoi(clone_node->first_attribute("num_clone")->value());
			infor->numClone = numClone;
			xml_node<>* clone_ChildNode = clone_node->first_node("CLONE");
			while (clone_ChildNode != 0 && numClone>0){
				//read name of clone
				string nameofClone = string(clone_ChildNode->first_attribute("name")->value());
				float x = atof(clone_ChildNode->first_attribute("x")->value());
				float y = atof(clone_ChildNode->first_attribute("y")->value());
				float z = atof(clone_ChildNode->first_attribute("z")->value());
				float userData = atof(clone_ChildNode->first_attribute("usedata")->value());
				infor->positionClone.push_back(PositionClone(nameofClone, x, y, z, userData));
				clone_ChildNode =clone_ChildNode->next_sibling("CLONE");
			}

			inforList.push_back(infor);
			entity_node = entity_node->next_sibling("ENTITY");
		}
		else{
			break;
		}
	} while (entity_node!=0);
	
}
vector<EntityInfor*> StateConfig::getConfig(){
	return this->inforList;
}