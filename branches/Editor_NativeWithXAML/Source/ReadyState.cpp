#include "pch.h"
#include "ReadyState.h"
#include "GamePlay.h"
#include "2DGameFramework\Utils.h"
#include "2DGameFramework\Sprite.h"
#include "2DGameFramework\ResourceManager.h"
#include "2DGameFramework\EntityFactory.h"
#include "2DGameFramework\InputHandler.h"
#include "2DGameFramework\StateManager.h"
//#include "2DGameFramework\TextRenderer.h"
//#include "2DGameFramework\SoundManager.h"
#include <string>

ReadyState::ReadyState()
{
	mName="Ready";
	path2Config=Global::gameResourceDir+string("Resources/Ready.txt");
	path2SoundList=Global::gameResourceDir+string("Resources/GamePlaySound.txt");
	bird=0;
	birdIsFly=false;
	pipelist.clear();
	/*TextRenderer* text=TextRenderer::GetInstance();
	text->InitFont("Resources//Fonts//04B_19__.ttf",25);*/
	score=0;
	isGameOver = false;
	offsetX = 0;
	countX = 0;
	offsetXPipe = 0;
}
ReadyState::~ReadyState()
{

}
void ReadyState::Init()
{
	const char* path2Config=this->path2Config.c_str();
	FILE* pFile=fopen(path2Config,"r");
	if (pFile==NULL)
		return;
	char buffer[MAX_LEN];

	ReadLine(pFile,buffer);
	int numEntity;
	int iPos=ScanNumberInt32(buffer,&numEntity,0);
	int iCount=0;
	//Pass Texure
	ReadLine(pFile,buffer);
	while(iCount<numEntity)
	{
		Sprite* sprite=0;

		ReadLine(pFile,buffer);
		char path[MAX_LEN];
		int t=ScanPath(buffer,path,0);
		Texture* texture=ResourceManager::GetInstance()->GetTexture(path);

		if (texture)
		{
			if (!texture->IsLoaded())
				texture->Load(Texture2D);

			//texture->Refenrence();

			//Entity Type, tự định nghĩa
			ReadLine(pFile,buffer);
#ifdef Win32
			sprite=(Sprite*)EntityFactory::CreateAEntity(buffer);
#endif
#ifdef Android
			int len=strlen(buffer);
			char*name1=new char[len];
			memcpy(name1,buffer,len);
			name1[len-1]='\0';
			string t1=string(name1);
			sprite=(Sprite*)EntityFactory::CreateAEntity(t1);
			delete[] name1;
#endif
#ifdef WindowPhone
			sprite = (Sprite*)EntityFactory::CreateAEntity(buffer);
#endif
			if (strstr(buffer,"Bird")!=0)
			{
				bird=(Bird*)sprite;
			}
			if (strstr(buffer,"Back")!=0)
			{
				back=sprite;
			}
			if (strstr(buffer,"Ground")!=0)
			{
				ground=sprite;
			}
			if (strstr(buffer,"Pipe")!=0)
			{
				pipe=sprite;
			}
			//Frame Sprite
			ReadLine(pFile,buffer);
			int iPos=0;
			float duration=0.0f,width=0.0f,height=0.0f;
			int numSpriteX=0,numSpriteY=0,numSprite=0;
			iPos=ScanNumberFloat32(buffer,&duration,iPos);
			iPos=ScanNumberFloat32(buffer,&width,iPos);
			iPos=ScanNumberFloat32(buffer,&height,iPos);
			iPos=ScanNumberInt32(buffer,&numSpriteX,iPos);
			iPos=ScanNumberInt32(buffer,&numSpriteY,iPos);
			iPos=ScanNumberInt32(buffer,&numSprite,iPos);

			//Shader
			char vShader[255];
			char fShader[255];
			ReadLine(pFile,buffer);
			iPos=ScanPath(buffer,vShader,0);
			ReadLine(pFile, buffer);
			iPos = ScanPath(buffer, fShader, 0);
			Shaders* shader=0;
			shader=ResourceManager::GetInstance()->GetShader(vShader, fShader);
			if (shader!=0)
			{
				if (shader->IsLoaded()==false)
				{
					shader->Load();
				}

				sprite->SetShader(shader);
				sprite->Init(texture,duration,width,height,numSpriteX,numSpriteY,numSprite);

				//Read number clone of sprite
				ReadLine(pFile,buffer);
				int numClone;
				ScanNumberInt32(buffer,&numClone,0);
				Sprite* prevSprite;
				for (int i=0;i<numClone;i++)
				{
					ReadLine(pFile,buffer);
					float x,y,z,flip;
					iPos=ScanNumberFloat32(buffer,&x,0);
					iPos=ScanNumberFloat32(buffer,&y,iPos);
					iPos=ScanNumberFloat32(buffer,&z,iPos);
					iPos=ScanNumberFloat32(buffer,&flip,iPos);
					Vector3 pos(x,y,z);
					if (sprite==0)
					{
						sprite=(Sprite*)prevSprite->Clone();
					}
					sprite->SetPosition(pos);	
					if (flip==1)
						sprite->FlipX();
					sprite->Play();
					mListEntity.push_back(sprite);

					prevSprite=sprite;
					sprite=0;
				}
			}
		}
		//ReadLine(pFile,buffer);
		iCount++;
	}

	bird->SetUpFrame(0,0,2);
	back->SetUpFrame(0,0,0);
	//pipe->SetUpFrame(0,0,0);
	ground2=ground->Clone();
	//ground->SetShowBoundEnable(true);
	mListEntity.push_back(ground2);
	Bound b=ground->GetBound();
	Bound b2=ground2->GetBound();
	ground->SetCanCollisionable(true);
	ground2->SetCanCollisionable(true);
	bird->SetCanCollisionable(true);
	//bird->SetShowBoundEnable(true);
	//ground->SetShowBoundEnable(true);
	//ground2->SetShowBoundEnable(true);
	Vector3 pos=ground->GetPosition();
	ground2->SetPosition(Vector3(pos.x + 28.0f-0.2f,b.bottomleft.y,2.1f));
	LoadSound();
	//InitPipe();
	//Key(32, true);
}

void ReadyState::Mouse(MouseData mouse, bool bIsDown)
{
	if (!mouse.isDown && mouse.type==Left)
	{
		if (isGameOver){
			GamePlayState *gameplay = new GamePlayState();
			StateManager::GetInstance()->SwitchState(gameplay, false);
			offsetX = 0;
			Vector3 pos = Global::currentCamera->GetPosition();
			Global::currentCamera->SetPosition(Vector3(0.0f, pos.y, pos.z));
		}
	}
}

void ReadyState::Key(unsigned char key, bool bIsPressed)
{
	Vector3 pos;
	switch(key)
	{ 
	case 'K':
		Global::currentCamera->MoveForward(0.1f);
		break;
	case 'M':
		Global::currentCamera->MoveBackward(0.1f);
		break;
	case 'A':
		Global::currentCamera->MoveLeft(0.2f);
		break;
	case 'D':
		Global::currentCamera->MoveRight(0.2f);

		break;
	case 'U':
		pos=Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x,pos.y+0.1f,pos.z));
		break;
	case 'V':
		pos=Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x,pos.y-0.1f,pos.z));
		break;
	case 32:
		if (bIsPressed)
		{
			if (isGameOver){
				GamePlayState *gameplay = new GamePlayState();
				StateManager::GetInstance()->SwitchState(gameplay, false);
				offsetX = 0;
				Vector3 pos = Global::currentCamera->GetPosition();
				Global::currentCamera->SetPosition(Vector3(0.0f, pos.y, pos.z));
			}
			if (hitted)
				return;
			birdIsFly=true;
			y0=bird->GetPosition().y;
			eslapedTime=0;
			rotateOffset=0.0f;
		}
		break;
	}
}

void ReadyState::onTouch(int id,float x,float y)
{
	if (id == Action::DOWN)
	{
		if (isGameOver){
			GamePlayState *gameplay = new GamePlayState();
			StateManager::GetInstance()->SwitchState(gameplay, false);
			offsetX = 0;
			Vector3 pos = Global::currentCamera->GetPosition();
			Global::currentCamera->SetPosition(Vector3(0.0f, pos.y, pos.z));
		}
	}
}


void ReadyState::Update(float deltatime)
{
	State::Update(deltatime);

	bird->MoveLinearForward(0.2f);
	if (bird->GetPosition().x>-7.0f){
		isGameOver = true;
		Global::currentCamera->MoveRight(0.2f);
		back->MoveLinearForward(0.2f);
		Vector3 camPos=Global::currentCamera->GetPosition();

		offsetX+=0.2f;
		if (offsetX>=28.0f)
		{
			if (countX%2==0)
			{
				Bound b=ground2->GetBound();
				Vector3 pos=ground2->GetPosition();
				ground->SetPosition(Vector3(pos.x+b.mWidth-0.2f,b.bottomleft.y,2.0f));
			}
			else
			{
				Bound b=ground->GetBound();
				Vector3 pos=ground->GetPosition();
				ground2->SetPosition(Vector3(pos.x+b.mWidth-0.2f,b.bottomleft.y,2.0f));
			}
			countX++;
			offsetX=0.2f;
		}
	}
}

void ReadyState::Draw()
{
#ifdef Win32
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
#endif
	State::Draw();
	/*TextRenderer* text=TextRenderer::GetInstance();
	text->RenderText("DEMO CHIM QUAY",-1.0f,0.8f,Vector4(1.0f,0.0f,0.0f,0.5f));
	if (bPastPipe)
	{
		char diem[5];
		sprintf(diem,"%d",score);
		text->RenderText(diem,-0.0f,-0.3f,Vector4(1.0f,1.0f,1.0f,1.0f));
	}*/
}

void ReadyState::RotateBird(float angle)
{
	if (angle==-3.14f/2.0f)
	{
		bird->SetRotateAngles(Vector3(0.0,0.0,angle));
		bird->SetScaleFactor(Vector3(0.7f,1.3f,1.0f));
		return;
	}
	Bound b=bird->GetBound();
	float xCenter=(b.topleft.x+b.topright.x)/2.0f;
	float yCenter=(b.topleft.y+b.bottomleft.y)/2.0f;
	Vector3 pos=bird->GetPosition();
	//bird->SetPosition(Vector3(xCenter,yCenter,1.5f*Entity2D::maxDepthOffset));
	bird->SetRotateAngles(Vector3(0.0,0.0,angle));
	//bird->SetPosition(Vector3(pos.x,pos.y,pos.z*Entity2D::maxDepthOffset));
	/*if (angle<-0.2f || angle>0.2f)
	{
	float scaleX=-(Abs(angle)-0.2f)-0.1f;
	float scaleY=Abs(angle)-0.2f+0.1f;
	bird->SetScaleFactor(Vector3(1.0f+scaleX,1.0+scaleY,1.0f));
	}*/
}

void ReadyState::ResumeSound()
{
	/*SoundManager* sound=SoundManager::getInstance();
	sound->PlayMusic(mListSoundName[0],true);*/
}