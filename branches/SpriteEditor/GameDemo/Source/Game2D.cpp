#include "pch.h"
#include "Game2D.h"
#include "GamePlay.h"
#include "MyStateConfig.h"
#include "2DGameFramework\StateManager.h"
#include "2DGameFramework\EntityFactory.h"

Game2D* Game2D::instance = 0;

Game2D* Game2D::GetInstance()
{
	if (instance == 0)
		instance = new Game2D();
	return instance;
}
#ifdef Win32
int Game2D::CreateGame(ESContext* esContext)
{
	Global::ScreenWidth = 600;
	Global::ScreenHeight = 600;


	if (Game::CreateGame(esContext) == 0)
	{
		GamePlayState* gamePlay = new GamePlayState();
		
		//gamePlay->SetStateConfig(new MyStateConfig());
		StateManager::GetInstance()->SwitchState(gamePlay, true);
		printf("CREATE GAME SUCCESS!");
		Global::currentCamera->SetPerspectiveProject(45.0f, 3.0 / 4, 1.0, 100000.0f);
		Vector3 pos;
		pos = Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x, 0, 30));
		//Global::currentCamera->SetRotationAngle(Vector3(0, 0, PI / 2));
		isInit = true;
		return 0;
	}
	return -1;
}
#endif

#ifdef Android
int Game2D::CreateGame()
{
	EntityFactory::RegisterEntity("Bird",new Bird());
	EntityFactory::RegisterEntity("Back",new Sprite());
	EntityFactory::RegisterEntity("Ground",new Sprite());
	EntityFactory::RegisterEntity("Pipe",new Sprite());
	if (Game::CreateGame()==0)
	{
		GamePlayState* gamePlay=new GamePlayState();
		StateManager::GetInstance()->AddState(gamePlay);
		StateManager::GetInstance()->SwitchState(gamePlay,true);
		printf("CREATE GAME SUCCESS!");
		Vector3 pos;
		Global::currentCamera->SetPerspectiveProject(45.0f,4.0/3,1.0,100000.0f);
		pos=Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x,0.5,18));
		Entity2D::maxDepthOffset=1000;
		isInit=true;
		return 0;
	}
	return -1;
}
#endif

#ifdef WindowPhone
int Game2D::CreateGame()
{
	EntityFactory::RegisterEntity("Bird", new Bird());
	EntityFactory::RegisterEntity("Back", new Sprite());
	EntityFactory::RegisterEntity("Ground", new Sprite());
	EntityFactory::RegisterEntity("Pipe", new Sprite());
	if (Game::CreateGame() == 0)
	{
		ReadyState* ready = new ReadyState();
		GamePlayState* gamePlay = new GamePlayState();
		StateManager::GetInstance()->AddState(ready);
		StateManager::GetInstance()->AddState(gamePlay);
		StateManager::GetInstance()->SwitchState(ready, true);
		printf("CREATE GAME SUCCESS!");
		Global::currentCamera->SetPerspectiveProject(45.0f, 4.0 / 3, 1.0, 100000.0f);
		Global::currentCamera->SetRotationAngle(Vector3(0, 0, 0));
		Vector3 pos;
		pos = Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(pos.x, 0.5, 19));
		Entity2D::maxDepthOffset = 1000;
		return 0;
	}
	return -1;
}
#endif