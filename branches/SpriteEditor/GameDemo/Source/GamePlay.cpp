#include "pch.h"
#include "GamePlay.h"
#include "2DGameFramework\Utils.h"
#include "2DGameFramework\Sprite.h"
#include "2DGameFramework\ResourceManager.h"
#include "2DGameFramework\EntityFactory.h"
#include "2DGameFramework\InputHandler.h"
#include "2DGameFramework\SoundManager.h"
#include "2DGameFramework\TextRenderer.h"
#include "2DGameFramework\StateManager.h"
#include <string>

GamePlayState::GamePlayState()
{
	mName = "GamePlay";
	path2Config = Global::gameResourceDir + string("Resources/GamePlay.xml");
	path2SoundList = Global::gameResourceDir + string("Resources/GamePlaySound.txt");
	TextRenderer* text = TextRenderer::GetInstance();
	text->InitFont("Resources//Fonts//04B_19__.ttf", 25);
	elapseTime = 0;
}
GamePlayState::~GamePlayState()
{

}
void GamePlayState::Init()
{
	State::Init();
	
	LoadSound();
	Key(32, true);
	isInit = true;
}

void GamePlayState::Mouse(MouseData mouse, bool bIsDown)
{
	if (!mouse.isDown && mouse.type == Left)
	{
		SoundManager::getInstance()->PlayEffect(mListSoundName[0], false);
	}
}

void GamePlayState::Key(unsigned char key, bool bIsPressed)
{
	Vector3 pos;
	switch (key)
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
		pos = Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x, pos.y + 0.1f, pos.z));
		break;
	case 'V':
		pos = Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x, pos.y - 0.1f, pos.z));
		break;
	case 32:
		if (bIsPressed)
		{
			
		}
		break;
	case 13:
		if (bIsPressed){
			GamePlayState* gamePlay = new GamePlayState();
			StateManager::GetInstance()->SwitchState(gamePlay, true);
		}
	}
}

void GamePlayState::onTouch(int id, float x, float y)
{
	if (id == Action::UP)
	{
	}
	if (id == Action::DOWN)
	{
		SoundManager::getInstance()->PlayEffect(mListSoundName[0], false);
	}
}


void GamePlayState::Update(float deltatime)
{
	State::Update(deltatime);
	elapseTime += deltatime;
	Vector3 pos;
	/*pos = Global::currentCamera->GetPosition();
	Global::currentCamera->SetPosition(Vector3(pos.x + 0.2f,pos.y,pos.z));*/
	//back->SetRotateAngles(Vector3(0, PI * elapseTime, 0));
}
void GamePlayState::Draw()
{
#ifdef Win32
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
#endif
	State::Draw();
}
void GamePlayState::ResumeSound()
{
#ifdef Win32
	SoundManager* sound = SoundManager::getInstance();
	sound->PlayMusic(mListSoundName[0], true);
#endif
}