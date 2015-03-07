#include "State.h"
#include "Utils.h"
#include "DebugDefine.h"
#include "AnimatedSprite.h"
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
	pickedEntity = 0;
}

State::~State()
{
	int n=mListEntity.size();
	for (int i=0;i<n;++i)
	{
		Delete(mListEntity[i]);
	}
	mListEntity.clear();
	n = mListSoundName.size();
	for (int i = 0; i < n; i++){
		SoundManager::getInstance()->Stop(mListSoundName[i]);
	}
	mListSoundName.clear();
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
#ifdef WindowPhone
	OutputDebugString(L"Begin Read Init");
#endif
	stateConfig->ReadConfig(path2Config.c_str());
#ifdef WindowPhone
	OutputDebugString(L"End Read Init");
#endif
	vector<EntityInfor*> inforList = stateConfig->getConfig();
	int numEntity = inforList.size();
	int iCount = 0;

	while (iCount < numEntity)
	{
		AnimatedSprite* sprite = 0;

		EntityInfor* infor = inforList[iCount];

		Texture* texture = ResourceManager::GetInstance()->GetTexture(infor->texturePath);
		string loadname;

		if (texture)
		{
			if (!texture->IsLoaded())
				texture->Load(Texture2D);
#if defined Win32 || defined WindowPhone
			sprite = (AnimatedSprite*)EntityFactory::CreateAEntity(infor->className);
#endif
#ifdef Android
			int len = strlen(infor->className);
			char*name1 = new char[len];
			memcpy(name1, infor->className, len);
			name1[len] = '\0';
			string t1 = string(name1);
			sprite = (AnimatedSprite*)EntityFactory::CreateAEntity(t1);
			delete[] name1;
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
				AnimatedSprite* prevSprite = NULL;
				
				for (int i = 0; i < numClone; i++)
				{
					PositionClone pos4 = infor->positionClone[i];
					Vector3 pos(pos4.x, pos4.y, pos4.z);

					if (sprite == 0)
					{
						sprite = (AnimatedSprite*)prevSprite->Clone();
					}
					sprite->SetPosition(pos);
					if (pos4.flip == 1){
						sprite->FlipX();
					}

					for (int j = 0; j < infor->registerFrame.size(); j++){
						char nFrame[MAX_LEN];
						strcpy(nFrame, infor->registerFrameName[j].c_str());
						vector<int> content = infor->registerFrame[nFrame];
						vector<int> ccontent = vector<int>();
						for (int t = 0; t < content.size(); t++){
							ccontent.push_back(content[t]);
						}

						sprite->addFrame(nFrame, ccontent);
					}

					sprite->Play();

					mListEntity.push_back(sprite);
					sprite->SetName(infor->positionClone[i].nameOfClone);
					entityList[infor->positionClone[i].nameOfClone] = sprite;
					prevSprite = sprite;
					sprite = 0;
				}
			}
		}

		iCount++;
	}
	isInit = true;
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
#if defined Win32 || defined Android
	glEnable(GL_DEPTH_TEST);
#endif
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


Entity2D* State::GetSelectedEntity(){
	return pickedEntity;
}

Entity2D* State::GetPickingEntity(MouseData mouse){
	vector<Entity2D*> tempList = mListEntity;
	std::sort(tempList.begin(), tempList.end(), CompareEntity2DByZ());

	int n = tempList.size();
	bool hasPickedItem=false;
	for (int i = 0; i < n; i++){
		if (tempList[i]->GetPosition().z>Global::currentCamera->GetPosition().z)
			continue;
		if (tempList[i]->IsPicked(mouse.position, Global::currentCamera)){
			pickedEntity = tempList[i];
			hasPickedItem = true;
			break;
		}
	}
	if (!hasPickedItem){
		return 0;
	}
	return pickedEntity;
}

void State::ReleaseSelectedEntity(){
	pickedEntity = 0;
}