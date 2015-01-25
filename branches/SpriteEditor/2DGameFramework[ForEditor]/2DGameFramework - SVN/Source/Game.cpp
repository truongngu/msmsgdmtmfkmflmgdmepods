#include "../Utilities/utilities.h"
#include "Game.h"
#include "ResourceManager.h"
#include "StateManager.h"
#include "Global.h"
#include "DebugDefine.h"
#include "Game.h"
#include "EntityFactory.h"
#include "InputHandler.h"
#include "SoundManager.h"
#include "TextRenderer.h"
#include "FPSLimiter.h"
#include "Utils.h"

Game* Game::instance=0;
vector<bool> Game::KeyDown=vector<bool>(false,100);
vector<string> Game::TouchDown=vector<string>();

Game::Game()
{
	isActive=false;
}

Game* Game::GetInstance()
{
	if (instance==0)
		instance=new Game();
	return instance;
}


#ifdef Win32
int Game::CreateGame(ESContext* esContext)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	string path=Global::gameResourceDir+Global::Path2Resource;
	if (resourceManager->Load(path)==0)
	{
		esInitContext(esContext);
		esCreateWindow(esContext, "New Game", Global::ScreenWidth, Global::ScreenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
		EntityFactory::InitEntity();
		InputHandler::Init();
		Global::currentCamera->SetPosition(Vector3(0.0f,1.0f,13.0f));
		Global::currentCamera->projection.SetPerspective(45.0f, 4.0f / 3, 1.0f, 10000000.0f);
		//fill(KeyDown.begin(),KeyDown.end(),false);
		Global::BoundShader->Init("Resources//Shaders//BoundVS.vsh","Resources//Shaders//BoundFS.fsh",0);
		Global::BoundShader->Load();
		Global::TextShader->Init("Resources//Shaders//Text.vsh","Resources//Shaders//Text.fsh",0);
		Global::TextShader->Load();
		Global::TextShaderInWorld->Init("Resources//Shaders//TextInWorld.vsh", "Resources//Shaders//Text.fsh", 0);
		Global::TextShaderInWorld->Load();
		Game* game = Game::GetInstance();
		game->isInit = true;
		game->Resume();
		return 0;
	}
	return -1;
}

void Game::Key(ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		InputHandler::SetKeyDown(key);
	}
	else
	{
		InputHandler::SetKeyUp(key);
	}
	switch (key){
	case 27:
		PostQuitMessage(0);
		break;
	}
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	Game* game=Game::GetInstance();

	if (currentState != NULL)
	{
			currentState->Key(key, bIsPressed);
	}
}

void Game::Mouse(ESContext* esContext, MouseData mouseData, bool bIsdown)
{
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	Game* game=Game::GetInstance();
	if (currentState != NULL)
	{
	/*	if (game->isActive)
		{*/
			currentState->Mouse(mouseData, bIsdown);
		/*}*/
	}
}

void Game::DrawFrame(ESContext *esContext,float deltaTime)
{
	FPSLimitter::GetInstance()->BeginFPS();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	Game* game=Game::GetInstance();
	if (currentState!=0)
	{
		if (game->isActive)
		{
			if (!currentState->IsRelease()&& currentState->IsInit())
				currentState->Update(deltaTime);
		}
		if (!currentState->IsRelease() && currentState->IsInit()){
			currentState->Draw();
			eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
		}
	}
	FPSLimitter::GetInstance()->EndFPS();
}

void Game::StartGame(ESContext* esContext)
{
	esRegisterUpdateFunc(esContext, DrawFrame);
	esRegisterMouseFunc(esContext, Mouse);
	esRegisterKeyFunc(esContext, Key);
	Game::GetInstance()->isActive=true;
	esMainLoop(esContext);
}
#endif

#ifdef Android
int Game::CreateGame()
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	string path=Global::gameResourceDir+Global::Path2Resource;
	if (resourceManager->Load(path)==0);
	{
		glViewport(0,0,Global::ScreenWidth,Global::ScreenHeight);
		EntityFactory::InitEntity();
		Global::currentCamera->SetPosition(Vector3(0.0f,0.0f,18.0f));
		Global::currentCamera->projection.SetPerspective(45.0f, 4.0f / 3, 1.0f, 10000000.0f);
		Global::BoundShader->Init("Resources//Shaders//BoundVS.vsh","Resources//Shaders//BoundFS.fsh",0);
		Global::BoundShader->Load();
		Global::TextShader->Init("Resources//Shaders//Text.vsh","Resources//Shaders//Text.fsh",0);
		Global::TextShader->Load();
		Global::TextShaderInWorld->Init("Resources//Shaders//TextInWorld.vsh","Resources//Shaders//Text.fsh",0);
		Global::TextShaderInWorld->Load();
		Game* game=Game::GetInstance();
		game->isInit=true;
		game->Resume();
		return 0;
	}
	return -1;
}

void Game::onTouch(int id,float x,float y)
{
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	Game* game=Game::GetInstance();
	if (currentState != NULL)
	{
		if (game->isActive)
		{
			if (!currentState->IsRelease()&& currentState->IsInit()){
				currentState->onTouch(id,x,y);
			}
		}
	}
}

void Game::DrawFrame(float deltaTime)
{
	FPSLimitter::GetInstance()->BeginFPS();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	Game* game=Game::GetInstance();
	if (currentState!=0)
	{
		if (game->isActive)
		{
			if (!currentState->IsRelease()&& currentState->IsInit()){
				currentState->Update(deltaTime);
			}
				
		}
		if (!currentState->IsRelease() && currentState->IsInit()){
			currentState->Draw();
		}
	}
	FPSLimitter::GetInstance()->EndFPS();
}

void Game::StartGame()
{
	DrawFrame(0.01f);

}
#endif

void Game::Pause()
{
	isActive=false;
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	if (currentState)
		currentState->PauseAllSound();
}


void Game::Resume()
{
	isActive=true;
	State* currentState=StateManager::GetInstance()->GetCurrentState();
	if (currentState)
		currentState->ResumeSound();
}

Game::~Game()
{
	
}

void Game::CleanMem()
{
	StateManager* sta=StateManager::GetInstance();
	Delete(sta);
	ResourceManager* res=ResourceManager::GetInstance();
	Delete(res);
	Delete(Global::currentCamera);
	Delete(Global::BoundShader);
	Delete(Global::TextShader);
	Delete(Global::TextShaderInWorld);
	EntityFactory::Close();
	KeyDown.clear();
	SoundManager* sound=SoundManager::getInstance();
	Delete(sound);
	TextRenderer* text=TextRenderer::GetInstance();
	Delete(text);
	FPSLimitter::Destroy();
	Delete(instance);
	Delete(Global::gWorld);
}

bool Game::IsInit()
{
	return isInit;
}
