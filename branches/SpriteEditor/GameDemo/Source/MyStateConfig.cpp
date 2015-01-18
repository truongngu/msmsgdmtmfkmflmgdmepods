#include "MyStateConfig.h"
#include "2DGameFramework\Utils.h"

MyStateConfig::MyStateConfig(){
	
}

MyStateConfig::~MyStateConfig(){

}

void MyStateConfig::ReadConfig(const char* path2Config){
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
		vector<std::string>* nameClone = new vector<std::string>();
		for (int i = 0; i<numClone; i++)
		{
			//Read name of each clone
			ReadLine(pFile, buffer);
			std::string nameOfClone = (std::string)buffer;
			nameClone->push_back(nameOfClone);
			//Read position infor of each clone
			ReadLine(pFile, buffer);
			float x, y, z, flip;
			iPos = ScanNumberFloat32(buffer, &x, 0);
			iPos = ScanNumberFloat32(buffer, &y, iPos);
			iPos = ScanNumberFloat32(buffer, &z, iPos);
			iPos = ScanNumberFloat32(buffer, &flip, iPos);

			infor->positionClone.push_back(PositionClone(x, y, z, flip));
		}

		infor->setAttribute("nameClone", nameClone);

		inforList.push_back(infor);
		iCount++;
	}
	fclose(pFile);
}