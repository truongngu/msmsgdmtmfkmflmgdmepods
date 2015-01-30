#include "Game2DDemo.h"

namespace Game2DDemo{
	GameInstance::GameInstance(){

	}
	
	GameInstance::~GameInstance(){
		ExitRequestHandle();
	}
	
	void GameInstance::SetHandleParent(HWND parent){
		SetEGLWindowParent(parent);
	}
	
	void GameInstance::CreategameWithHandle(void* hWnd){
		SetHandleParent(HWND(hWnd));
		Creategame();
	}
	
	void GameInstance::Creategame(){
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		Game2D* game = Game2D::GetInstance();

		if (game->CreateGame(&esContext) != 0)
		{
			Delete(game);
			Game::CleanMem();
			return;
		}
		game->StartGame(&esContext);
		/*Delete(game);
		Game::CleanMem();*/
	}
	
	void GameInstance::Rungame(float deltaTime){
		Game2D* game = Game2D::GetInstance();
		if (game->IsInit())
		{
			game->DrawFrame(&esContext, deltaTime);
		}
	}
	
	void GameInstance::ExitRequestHandle(){
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		Game2D* game = Game2D::GetInstance();
		if (game->IsInit()){
			game->Pause();
			Delete(game);
			Game::CleanMem();
		}
	}
	
	void GameInstance::SetWindowSizeChange(int x, int y, int w, int h){
		ChangeEGLWindowSize(x, y, w, h);
	}
	
	void GameInstance::SetHandleLocaltion(int x, int y, int w, int h){
		Global::ScreenWidth = w;
		Global::ScreenHeight = h;
		SetEGLWindowLocation(x, y, w, h);
	}

	void GameInstance::KeyDownRequestHandle(unsigned char key){
		Game2D* game = Game2D::GetInstance();
		if (game->IsInit())
		{
			game->Key(&esContext, key, true);
		}
	}
	
	void GameInstance::KeyUpRequestHandle(unsigned char key){
		Game2D* game = Game2D::GetInstance();
		if (game->IsInit())
		{
			game->Key(&esContext, key, false);
		}
	}

	void GameInstance::MouseRequestHandle(bool isDown, int x, int y, int type){
		Game2D* game = Game2D::GetInstance();
		if (game->IsInit())
		{
			MouseData mouseData = MouseData();
			mouseData.isDown = isDown;
			mouseData.position.x = x;
			mouseData.position.y = y;
			switch (type)
			{
			case 0:
				mouseData.type = MouseType::NotClicked;
				break;
			case 1:
				mouseData.type = MouseType::Left;
				break;
			case 2:
				mouseData.type = MouseType::Middle;
				break;
			case 3:
				mouseData.type = MouseType::Right;
				break;
			default:
				mouseData.type = MouseType::NotClicked;
				break;
			}
			game->Mouse(&esContext, mouseData, isDown);
		}
	}

	void GameInstance::MoveCameraTo(float x, float y){
		Vector3 pos;
		pos = Global::currentCamera->GetPosition();
		Global::currentCamera->SetPosition(Vector3(x, y, pos.z));
	}

	void GameInstance::MoveSelectedEntity(float deltax, float deltay){
		State* curState = StateManager::GetInstance()->GetCurrentState();
		if (curState && curState->IsInit()){

			Sprite* picked = 0;
			State* curState = StateManager::GetInstance()->GetCurrentState();
			if (curState && curState->IsInit()){
				picked = (Sprite*)curState->GetSelectedEntity();
				if (picked){
					Vector3 pos;
					pos = picked->GetPosition();
					picked->SetPosition(Vector3(pos.x + deltax, pos.y + deltay, pos.z));
				}
			}
		}
	}

	int GameInstance::MoveTheSelectedEntity(String^ name, float* eX, float* eY, float deltax, float deltay){
		State* curState = StateManager::GetInstance()->GetCurrentState();
		Sprite* picked = 0;
		if (curState && curState->IsInit()){
			State* curState = StateManager::GetInstance()->GetCurrentState();
			if (curState && curState->IsInit()){
				picked = (Sprite*)curState->GetSelectedEntity();
				if (picked){
					Vector3 pos;
					pos = picked->GetPosition();
					picked->SetPosition(Vector3(pos.x + deltax, pos.y + deltay, pos.z));
					pos = picked->GetPosition();
					*eX = pos.x;
					*eY = pos.y;

					name = gcnew String(picked->GetName().c_str());
				}
			}
		}

		return picked != 0;
	}

	MyResult^ GameInstance::MoveTheSelectedEntity(float deltax, float deltay){
		State* curState = StateManager::GetInstance()->GetCurrentState();
		Sprite* picked = 0;
		MyResult^ result = gcnew MyResult();
		if (curState && curState->IsInit()){
			State* curState = StateManager::GetInstance()->GetCurrentState();
			if (curState && curState->IsInit()){
				picked = (Sprite*)curState->GetSelectedEntity();
				if (picked){
					Vector3 pos;
					pos = picked->GetPosition();
					picked->SetPosition(Vector3(pos.x + deltax, pos.y + deltay, pos.z));
					pos = picked->GetPosition();
					result->SetX(pos.x);
					result->SetY(pos.y);
					result->SetName(picked->GetName());
				}
			}
		}
		result->Result(picked != 0);
		return result;
	}

	int GameInstance::GetPickingEntity(String^ name, float x, float y){
		MouseData mouse;
		mouse.position.x = x;
		mouse.position.y = y;
		/*string pickAt = "Game2D: Pick at " + std::to_string(x) + "," + std::to_string(y);
		Log(pickAt.c_str());*/
		string res = "";
		Sprite* picked = 0;
		State* curState = StateManager::GetInstance()->GetCurrentState();
		if (curState && curState->IsInit()){
			picked = (Sprite*)curState->GetPickingEntity(mouse);

		}
		if (picked != 0){
			picked->SetShowBoundEnable(true);
			res = picked->GetName();
			Log(res.c_str());
			Log(stringify(res.length()).c_str());
		}
		name = gcnew String(res.c_str());
		//strcpy((char*)name, res.c_str());
		Log(res.c_str());

		return picked != 0;
	}

	MyResult^ GameInstance::GetPickingEntity(float x, float y){
		MouseData mouse;
		mouse.position.x = x;
		mouse.position.y = y;
		/*string pickAt = "Game2D: Pick at " + std::to_string(x) + "," + std::to_string(y);
		Log(pickAt.c_str());*/
		string res = "";
		Sprite* picked = 0;
		MyResult^ result = gcnew MyResult();
		State* curState = StateManager::GetInstance()->GetCurrentState();
		if (curState && curState->IsInit()){
			picked = (Sprite*)curState->GetPickingEntity(mouse);

		}
		if (picked != 0){
			picked->SetShowBoundEnable(true);
			res = picked->GetName();
			Log(res.c_str());
			Log(stringify(res.length()).c_str());
			result->SetName(picked->GetName());
		}
		
		Log(res.c_str());

		result->Result(picked != 0);
		return result;
	}

	void GameInstance::ReleaseSelectedEntity(){

		State* curState = StateManager::GetInstance()->GetCurrentState();
		if (curState && curState->IsInit()){

			Sprite* picked = 0;
			State* curState = StateManager::GetInstance()->GetCurrentState();
			if (curState && curState->IsInit()){
				picked = (Sprite*)curState->GetSelectedEntity();
				if (picked)
					picked->SetShowBoundEnable(false);
				curState->ReleaseSelectedEntity();
			}
		}
	}

	void* GameInstance::GetContextHandle(){
		return esContext.hWnd;
	}

	void GameInstance::ExecuteGame(LPCWSTR method, LPVOID param){

		if (wcsstr(method, (LPCWSTR)"Rungame")){
			MessageBox(esContext.hWnd, wcsstr(method, (LPCWSTR)"R"), (LPCWSTR)method, MB_OK);

		}
	}

	MyResult::MyResult(){
		x = 0;
		y = 0;
	}
	
	MyResult::MyResult(float X){
		x = X;
		y = X;
	}
	
	MyResult::MyResult(float X, float Y){
		x = X;
		y = Y;
	}

	float MyResult::GetX(){ return x; }
	void MyResult::SetX(float X){ x = X; }
	float MyResult::GetY(){ return y; }
	void MyResult::SetY(float Y){ y = Y; }
	void MyResult::SetName(std::string Name){
		name = gcnew String(Name.c_str());
	}
	String^ MyResult::GetName(){
		return name;
	}
	int MyResult::GetResult(){
		return result;
	}
	void MyResult::Result(int Result){
		result = Result;
	}
}