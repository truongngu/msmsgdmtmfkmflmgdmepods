#include "New.h"
#include "ReadyState.h"
#include "2DGameFramework\Utils.h"
#include "2DGameFramework\Sprite.h"
#include "2DGameFramework\ResourceManager.h"
#include "2DGameFramework\EntityFactory.h"
#include "2DGameFramework\InputHandler.h"
#include "2DGameFramework\SoundManager.h"
#include "2DGameFramework\TextRenderer.h"
#include "2DGameFramework\StateManager.h"
#include <string>

Gamemoi::GameMoi()
{
	mName="Gamemoi";
	path2Config=Global::gameResourceDir+string("Resources/GamePlay.txt");
	path2SoundList=Global::gameResourceDir+string("Resources/GamePlaySound.txt");
	bird=0;
	birdIsFly=false;
	pipelist.clear();
	TextRenderer* text=TextRenderer::GetInstance();
	text->InitFont("Resources//Fonts//04B_19__.ttf",25);
	score=0;
	isGameOver = false;
	offsetX = 0;
	countX = 0;
	offsetXPipe = 0;
}
Gamemoi::~Gamemoi()
{

}
void Gamemoi::Init()
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
			char*name1=Gamemoi char[len];
			memcpy(name1,buffer,len);
			name1[len-1]='\0';
			string t1=string(name1);
			sprite=(Sprite*)EntityFactory::CreateAEntity(t1);
			delete[] name1;
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
			ReadLine(pFile,buffer);
			iPos=ScanPath(buffer,path,0);
			Shaders* shader=0;
			shader=ResourceManager::GetInstance()->GetShader(path);
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
	pipe->SetUpFrame(0,0,0);
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
	InitPipe();
	Key(32, true);
}

void Gamemoi::InitPipe()
{
	pipe->SetCanCollisionable(true);
	//pipe->SetShowBoundEnable(true);
	pipelist.push_back(pipe);
	Sprite* pc=pipe->Clone();
	pc->SetPosition(Vector3(8.0f,8.0f,2.0f));
	pc->SetUpFrame(1,1,1);
	pipelist.push_back(pc);
	mListEntity.push_back(pc);
	//pc->SetShowBoundEnable(true);

	float x=8.0f+GetARandomNumber()%20+10.0f;
	float y=GetARandomNumber()%5+10.0f;
	pc=pipe->Clone();
	pc->SetPosition(Vector3(x,-y,2.0f));
	pc->SetUpFrame(0,0,0);
	pipelist.push_back(pc);
	mListEntity.push_back(pc);

	y=GetARandomNumber()%8+5.0f;
	pc=pipe->Clone();
	pc->SetPosition(Vector3(x,y,2.0f));
	pc->SetUpFrame(1,1,1);
	pipelist.push_back(pc);
	mListEntity.push_back(pc);

	x=x+GetARandomNumber()%20+10.0f;
	y=GetARandomNumber()%5+10.0f;
	pc=pipe->Clone();
	pc->SetPosition(Vector3(x,-y,2.0f));
	pc->SetUpFrame(0,0,0);
	pipelist.push_back(pc);
	mListEntity.push_back(pc);

	y=GetARandomNumber()%8+5.0f;
	pc=pipe->Clone();
	pc->SetPosition(Vector3(x,y,2.0f));
	pc->SetUpFrame(1,1,1);
	pipelist.push_back(pc);
	mListEntity.push_back(pc);
	pipeIndex=0;
	maxX=x;
	hitted=false;

	for (int i=0;i<pipelist.size();i++){
		pipelist[i]->SetCanCollisionable(false);
		pipelist[i]->SetIsActiveInGame(false);
	}
}

void Gamemoi::Mouse(MouseData mouse, bool bIsDown)
{
	if (!mouse.isDown && mouse.type==Left)
	{
		if (isGameOver){
			ReadyState *ready = Gamemoi ReadyState();
			StateManager::GetInstance()->AddState(ready);
			StateManager::GetInstance()->SwitchState(ready, false);
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
		//bird->GoUp(6.0f);
		SoundManager::getInstance()->PlayEffect(mListSoundName[0],false);
	}
}

void Gamemoi::Key(unsigned char key, bool bIsPressed)
{
	//Vector3 pos;
	switch(key)
	{ 
		//case 'K':
		//	Global::currentCamera->MoveForward(0.1f);
		//	break;
		//case 'M':
		//	Global::currentCamera->MoveBackward(0.1f);
		//	break;
		//case 'A':
		//	Global::currentCamera->MoveLeft(0.2f);
		//	break;
		//case 'D':
		//	Global::currentCamera->MoveRight(0.2f);
		//	
		//break;
		//case 'U':
		//	pos=Global::currentCamera->GetPosition();
		//	Global::currentCamera->SetPosition(Vector3(pos.x,pos.y+0.1f,pos.z));
		//	break;
		//case 'V':
		//	 pos=Global::currentCamera->GetPosition();
		//	Global::currentCamera->SetPosition(Vector3(pos.x,pos.y-0.1f,pos.z));
		//	break;
	case 32:
		if (bIsPressed)
		{
			if (hitted)
				return;
			birdIsFly=true;
			y0=bird->GetPosition().y;
			eslapedTime=0;
			rotateOffset=0.0f;
			//bird->GoUp(6.0f);
			SoundManager::getInstance()->PlayEffect(mListSoundName[0],false);
		}
		break;
	case 13:
		if (bIsPressed){
			if (isGameOver){
				ReadyState *ready = Gamemoi ReadyState();
				StateManager::GetInstance()->AddState(ready);
				StateManager::GetInstance()->SwitchState(ready, false);
				offsetX = 0;
				Vector3 pos = Global::currentCamera->GetPosition();
				Global::currentCamera->SetPosition(Vector3(0.0f, pos.y, pos.z));
			}
		}
	}
}

void Gamemoi::onTouch(int id,float x,float y)
{
	if (id==Action::UP)
	{
		if (hitted)
			return;
		birdIsFly=true;
		y0=bird->GetPosition().y;
		eslapedTime=0;
		rotateOffset=0.0f;
		//bird->GoUp(6.0f);
		SoundManager::getInstance()->PlayEffect(mListSoundName[0],false);
	}
}


void Gamemoi::Update(float deltatime)
{
	State::Update(deltatime);
	bool hit=false;
	for (int i=0;i<6;i++)
		if (bird->CollisWith(pipelist[i]))
		{
			hit=true;
			break;
		}
		if (bird->CollisWith(ground) || bird->CollisWith(ground2))
		{
			hit=true;
			//return;
		}

		if (hit)
		{
			RotateBird(-3.14f/2.0f);
			birdIsFly=true;
			y0=bird->GetPosition().y;
			eslapedTime=0;
			rotateOffset=0.0f;
			hitted=true;
			bird->SetCanCollisionable(false);
			SoundManager::getInstance()->PlayEffect(mListSoundName[2],false);
		}
		else if (!hitted)
		{
			bird->MoveLinearForward(0.2f);
			if (bird->GetPosition().x>-7.0f){
				Global::currentCamera->MoveRight(0.2f);
				back->MoveLinearForward(0.2f);
				Vector3 camPos=Global::currentCamera->GetPosition();
				for (int i=0;i<6;i++)
				{
					if (camPos.x-13.0f >pipelist[i]->GetBound().topright.x)
					{
						pipeIndex=i;
						CreatePipe();
						break;
					}
					if (bird->GetPosition().x-pipelist[i]->GetBound().topright.x<=0.3f && bird->GetPosition().x-pipelist[i]->GetBound().topright.x>0.1f)
					{
						if (pipelist[i]->IsActiveInGame()){
							SoundManager::getInstance()->PlayEffect(mListSoundName[1],false);
							bPastPipe=true;
							score++;
						}
						break;
					}
				}

				offsetX+=0.2f;
				if (offsetX>=28.0f)
				{
					if (countX%2==0)
					{
						Bound b=ground2->GetBound();
						Vector3 pos=ground2->GetPosition();
						ground->SetPosition(Vector3(pos.x+b.Width()-0.2f,b.bottomleft.y,2.0f));
					}
					else
					{
						Bound b=ground->GetBound();
						Vector3 pos=ground->GetPosition();
						ground2->SetPosition(Vector3(pos.x+b.Width()-0.2f,b.bottomleft.y,2.0f));
					}
					countX++;
					offsetX=0.2f;
				}
			}
		}

		if (birdIsFly)
		{
			int i;
			//bird->GoUp(5.0f);
			Vector3 birdpos = bird->GetPosition();
			this->eslapedTime+=deltatime*2.0f;
			birdpos.y=y0+5.0f*(eslapedTime)-0.5f*9.8*(eslapedTime)*(eslapedTime);
			bird->SetPosition(Vector3(birdpos.x,birdpos.y,birdpos.z*Entity2D::maxDepthOffset));

#pragma region XuliChim
			float y=bird->GetPosition().y;
			if (y>y0 && !hitted)
			{
				if (rotateOffset<0.2f)
					rotateOffset+=0.05f;
				else
					rotateOffset-=0.05f;
				RotateBird(rotateOffset);

			}
			else if (!hitted)
			{
				if (rotateOffset>-0.2f)
					rotateOffset-=0.08f;
				RotateBird(rotateOffset);

			}
			/*Vector3 pos=bird->GetPosition();
			bird->SetPosition(Vector3(pos.x,y,pos.z*Entity2D::maxDepthOffset));*/
			if (hitted )
			{
				Bound b=ground->GetBound();
				if (bird->GetPosition().y<=b.topleft.y)
				{
					Vector3 pos=bird->GetPosition();
					bird->SetPosition(Vector3(pos.x,b.topleft.y+2.0f,pos.z*Entity2D::maxDepthOffset));
					bird->SetUpFrame(2,2,2);
					isGameOver = true;
				}
			}
#pragma endregion


		}
}

void Gamemoi::Draw()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	State::Draw();
	TextRenderer* text=TextRenderer::GetInstance();
	text->RenderText("DEMO CHIM QUAY",-1.0f,0.8f,Vector4(1.0f,0.0f,0.0f,0.5f));
	if (bPastPipe)
	{
		char diem[5];
		sprintf(diem,"%d",score);
		text->RenderText(diem,-0.0f,-0.3f,Vector4(1.0f,1.0f,1.0f,1.0f));
	}
}
bool DiemThuocHCN(Vector2 diem,Bound hcn)
{
	if((hcn.topleft.x<=diem.x&&hcn.topright.x>=diem.x)||(hcn.topright.x<=diem.x&&hcn.topleft.x>=diem.x))
		if((hcn.bottomleft.y<=diem.y&&hcn.topleft.y>=diem.y)||(hcn.topleft.y<=diem.y&&hcn.bottomleft.y>=diem.y))
			return true;
	return false;
}
bool Gamemoi::CheckCollision(Bound b1, Bound b2){
	if(DiemThuocHCN(b1.topleft,b2)||DiemThuocHCN(b2.topleft,b1))
		return true;
	if(DiemThuocHCN(b1.topright,b2)||DiemThuocHCN(b2.topright,b1))
		return true;
	if(DiemThuocHCN(b1.bottomleft,b2)||DiemThuocHCN(b2.bottomleft,b1))
		return true;
	if(DiemThuocHCN(b1.bottomright,b2)||DiemThuocHCN(b2.bottomright,b1))
		return true;
	return false;
}


void Gamemoi::CreatePipe()
{
	pipelist[pipeIndex]->SetCanCollisionable(true);
	pipelist[pipeIndex]->SetIsActiveInGame(true);
	pipelist[pipeIndex + 1]->SetCanCollisionable(true);
	pipelist[pipeIndex + 1]->SetIsActiveInGame(true);

	float x=maxX+GetARandomNumber()%5 +10.0f;
	maxX=x;
	float y1 =GetARandomNumber()%5+10.0f;
	float y2=GetARandomNumber()%7+2.0f;
	pipelist[pipeIndex]->SetPosition(Vector3(x,-y1,1.9f));

	Bound b=pipelist[pipeIndex]->GetBound();
	y2=GetARandomNumber()%3+3.0f+b.topleft.y;
	if (y2<3.0f)
		y2=5.0f;
	pipelist[pipeIndex+1]->SetPosition(Vector3(x,y2,1.9f));
	pipeIndex++;
	if (pipeIndex==3)
	{
		pipeIndex=0;
	}
}

void gamemoi::RotateBird(float angle)
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

	bird->SetRotateAngles(Vector3(0.0,0.0,angle));
}
void Gamemoi::ResumeSound()
{
	SoundManager* sound=SoundManager::getInstance();
	sound->PlayMusic(mListSoundName[0],true);
}