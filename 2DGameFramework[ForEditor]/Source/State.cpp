#include "State.h"
#include "Utils.h"
#include "DebugDefine.h"
#include "Sprite.h"
#include "EntityFactory.h"
#include "ResourceManager.h"
#include "Global.h"
#include "SoundManager.h"

State::State()
{
	mListEntity.clear();
	mListSoundName.clear();
	mName="";
	isRelease=false;
	isInit=false;
	nextState = 0;
	stateConfig = new StateConfig();
}

State::~State()
{
	int n=mListEntity.size();
	for (int i=0;i<n;++i)
	{
		Delete(mListEntity[i]);
	}
	mListEntity.clear();
	Delete(stateConfig);
}

StateConfig* State::GetStateConfig(){ return stateConfig; }
void State::SetStateConfig(StateConfig* newConfig){
	if (newConfig != 0)
	{ 
		Delete(stateConfig);
		stateConfig = newConfig;
	} 
}

void State::Init(){
	stateConfig->ReadConfig(path2Config.c_str());

	vector<EntityInfor*> inforList = stateConfig->getConfig();
	int numEntity = inforList.size();
	int iCount = 0;

	while (iCount < numEntity)
	{
		Sprite* sprite = 0;

		EntityInfor* infor = inforList[iCount];

		Texture* texture = ResourceManager::GetInstance()->GetTexture(infor->texturePath);
		string loadname;

		if (texture)
		{
			if (!texture->IsLoaded())
				texture->Load(Texture2D);
#ifdef Win32
			sprite = (Sprite*)EntityFactory::CreateAEntity(infor->className);
#endif
#ifdef Android
			int len = strlen(infor.className);
			char*name1 = new char[len];
			memcpy(name1, infor.className, len);
			name1[len - 1] = '\0';
			string t1 = string(name1);
			sprite = (Sprite*)EntityFactory::CreateAEntity(t1);
			delete[] name1;
#endif
#ifdef WindowPhone
			sprite = (Sprite*)EntityFactory::CreateAEntity(infor.className);
#endif			
			Shaders* shader = 0;
			shader = ResourceManager::GetInstance()->GetShader(infor->vShaderPath, infor->fShaderPath);
			if (shader != 0)
			{
				if (shader->IsLoaded() == false)
				{
					shader->Load();
				}

				sprite->SetShader(shader);
				sprite->Init(texture, infor->duration, infor->width, infor->height
					, infor->numSpriteX, infor->numSpriteY, infor->numSprite);

				int numClone = infor->numClone;
				Sprite* prevSprite = NULL;
				
				for (int i = 0; i < numClone; i++)
				{
					PositionClone pos4 = infor->positionClone[i];
					Vector3 pos(pos4.x, pos4.y, pos4.z);

					if (sprite == 0)
					{
						sprite = (Sprite*)prevSprite->Clone();
					}
					sprite->SetPosition(pos);
					if (pos4.flip == 1){
						sprite->FlipX();
					}
					sprite->Play();

					mListEntity.push_back(sprite);
					entityList[infor->positionClone[i].nameOfClone] = sprite;
					prevSprite = sprite;
					sprite = 0;
				}
			}
		}

		iCount++;
	}
}

void State::Update(float deltatime)
{
	int n=mListEntity.size();
	mListEntityInView.clear();
	for (int i=0;i<n;i++)
	{
		mListEntity[i]->update(deltatime);
		//printf("Finish update entity %d \n",i);
		if (IsInCameraView(mListEntity[i]))
		{
			mListEntityInView.push_back(mListEntity[i]);
		}
	}
}

void State::Draw()
{
	
	int n=mListEntityInView.size();
	for (int i=0;i<n;i++)
	{
		mListEntityInView[i]->draw();
	}
	/*int n=mListEntity.size();
	for (int i=0;i<n;i++)
	{
		mListEntity[i]->draw();
	}*/
}

void State::LoadSound()
{
	const char* path2Sound=this->path2SoundList.c_str();
	FILE* pFile=fopen(path2Sound,"r");
	if (pFile==0)
		return;
	char buffer[MAX_LEN];
	SoundManager* sound=SoundManager::getInstance();
	if (!sound)
		return;
	while(strstr(buffer,"End")==0)
	{
		ReadLine(pFile,buffer);
		if (strstr(buffer,"//End")!=0)
			return;
		char path[MAX_LEN];
		int t=ScanPath(buffer,path,0);
		char soundPath[MAX_LEN];
		strcpy(soundPath,Global::gameResourceDir.c_str());
		strcat(soundPath,path);
		if (strstr(soundPath,"wav"))
		{
			sound->LoadWAV(soundPath);
		}
		else if (strstr(soundPath,"ogg"))
		{
			sound->LoadOGG(soundPath);
		}
		mListSoundName.push_back(string(soundPath));
	}
}

void State::PauseAllSound()
{
	int n=mListSoundName.size();
	SoundManager*sound=SoundManager::getInstance();
	for (int i=0;i<n;i++)
		sound->Pause(mListSoundName[i]);
}

State* State::GetNextState(){
	return nextState;
}

void State::SetNextState(State* value){
	nextState = value;
}

Entity2D* State::GetEntityByName(std::string name){
	return entityList[name];
}

Entity2D* State::GetEntityByIndex(int i){
	return mListEntity[i];
}