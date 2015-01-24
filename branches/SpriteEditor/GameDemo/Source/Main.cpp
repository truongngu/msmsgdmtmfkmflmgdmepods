#include <stdio.h>
#include "2DGameFramework\DebugDefine.h"
#include "Game2D.h"
#include "Utilities\esUtil_win.h"

#ifdef Win32
#include "crtdbg.h"
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include "2DGameFramework\State.h"
#include "2DGameFramework\StateManager.h"
#include "2DGameFramework\Utils.h"

ESContext esContext;
HANDLE  hThreadArray[1];
#endif
#ifdef Android
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "2DGameFramework\Global.h"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" {
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onTouch(JNIEnv * env, jobject obj,  jint id, jfloat x, jfloat y);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_pauseApp(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resumeApp(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_exitApp(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setResourcePath(JNIEnv * env, jobject obj,jstring path,jobject assetmanager,jstring path2Cache);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	Game2D* mainGame = Game2D::GetInstance();

	if (!mainGame->IsInit())
	{
		char so[2];
		sprintf(so,"%d",width);
		LOGE(so);
		sprintf(so,"%d",height);
		LOGE(so);
		if(height>width)
		{
			Global::ScreenWidth=height;
			Global::ScreenHeight=width;
		}
		else
		{
			Global::ScreenWidth=width;
			Global::ScreenHeight=height;
		}
		LOGE("INiting!");
		if (mainGame->CreateGame() != 0)
		{
			delete Global::currentCamera;
			Delete(mainGame);
			LOGE("Error!");
			return;
		}
	}
	else
	{
		LOGE("GAME HAS INIT");
	}
	LOGE("ON CHANGE SURFACE");
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setResourcePath(JNIEnv * env, jobject obj,jstring path, jobject assetmanager,jstring path2Cache)
{
	const char *systemPath = env->GetStringUTFChars(path, JNI_FALSE);

	Global::gameResourceDir=string(systemPath);
	Global::gameResourcePath=Global::gameResourceDir+"Resources/";
	LOGE("SYSTEM PATH:");
	LOGE(Global::gameResourceDir.c_str());
	string path2Res=Global::gameResourcePath;
	path2Res=path2Res+"Res.txt";
	LOGE(path2Res.c_str());
	FILE* out = fopen(path2Res.c_str(), "r");
	if (out)
		LOGE("FILE EXIST");
	else
		LOGE("FILE NOT FOUND");
	env->ReleaseStringUTFChars(path, systemPath);
}



JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{

	Game2D* mainGame = Game2D::GetInstance();
	mainGame->DrawFrame(0.03f);

}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_pauseApp(JNIEnv * env, jobject obj)
{

	Game2D* mainGame = Game2D::GetInstance();
	mainGame->Pause();

}


JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resumeApp(JNIEnv * env, jobject obj)
{
	Game2D* mainGame = Game2D::GetInstance();
	mainGame->Resume();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_exitApp(JNIEnv * env, jobject obj)
{
	LOGE("BEGIN EXIT");
	Game2D* mainGame = Game2D::GetInstance();
	delete Global::currentCamera;
	LOGE("DELETE CAMERA");
	Delete(mainGame);
	LOGE("DELETE GAME");
	Game::CleanMem();
}


void onTouch(int id, float x, float y)
{
	Game2D* mainGame = Game2D::GetInstance();
	mainGame->onTouch(id,x,y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onTouch(JNIEnv * env, jobject obj,  jint id, jfloat x,jfloat y)
{
	//LOGE("TOUCH");
	onTouch(id,x,y);
}
#endif

#ifdef Win32
extern "C"
{
	__declspec(dllexport)
		void SetHandleParent(HWND parent){
			SetEGLWindowParent(parent);
		}
}
extern "C"
{
	__declspec(dllexport)
		void SetHandleLocaltion(int x, int y, int w, int h){
			SetEGLWindowLocation(x, y, w, h);
		}
}
extern "C"
{
	__declspec(dllexport)
		void RefreshRequestHandle(){
			if (esContext.hWnd)
				esContext.keyFunc(&esContext, 13, true);
			/*Game2D* game = Game2D::GetInstance();
			if (game)
			game->Key(&esContext, 13, true);*/
		}
}
extern "C"
{
	__declspec(dllexport)
		void KeyRequestHandle(unsigned char key){
			if (esContext.hWnd)
				esContext.keyFunc(&esContext, key, true);
			/*Game2D* game = Game2D::GetInstance();
			if (game)
			game->Key(&esContext, 13, true);*/
		}
}
extern "C"
{
	__declspec(dllexport)
		void MouseRequestHandle(bool isDown, int x, int y, int type){
			if (esContext.hWnd){
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
				esContext.mouseFunc(&esContext, mouseData, isDown);
			}
		}
}
extern "C"
{
	__declspec(dllexport)
		void ExitRequestHandle(){
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			Game2D* game = Game2D::GetInstance();
			if (game->IsInit()){
				game->Pause();
				Delete(game);
				Game::CleanMem();
			}
		}
}

extern "C"
{
	__declspec(dllexport)
		void SetWindowSizeChange(int x, int y, int w, int h){
			ChangeEGLWindowSize(x, y, w, h);
		}
}

extern "C"
{
	__declspec(dllexport)
		void MoveCameraTo(float x, float y){
			Vector3 pos;
			pos = Global::currentCamera->GetPosition();
			Global::currentCamera->SetPosition(Vector3(x, y, pos.z));
		}
}

extern "C"
{
	__declspec(dllexport)
		void MoveSelectedEntity(float deltax, float deltay){
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
}




extern "C"
{
	__declspec(dllexport)
		void GetPickingEntity(WCHAR* name,float x, float y){
			MouseData mouse;
			mouse.position.x = x;
			mouse.position.y = y;
			string pickAt = "Game2D: Pick at " + std::to_string(x) + "," + std::to_string(y);
			Log(pickAt.c_str());
			string res = "";
			Sprite* picked=0;
			State* curState = StateManager::GetInstance()->GetCurrentState();
			if (curState && curState->IsInit()){
				picked = (Sprite*)curState->GetPickingEntity(mouse);
				
			}
			if (picked != 0){
				picked->SetShowBoundEnable(true);
				res=picked->GetName();
				Log(res.c_str());

			}
			memcpy(name, res.c_str(), res.length());
			name[res.length()] = '\0';
		}
}

extern "C"
{
	__declspec(dllexport)
		void ReleaseSelectedEntity(){
		
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
}

extern "C"
{
	__declspec(dllexport)
		void Rungame(){
			ExecuteEGLWindow("UpdateGame", 0);
		}
}

extern "C"
{
	__declspec(dllexport)
		void Creategame(){
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			Game2D* game = Game2D::GetInstance();

			if (game->CreateGame(&esContext) != 0)
			{
				Delete(game);
				return;
			}
			game->StartGame(&esContext);
			/*Delete(game);
			Game::CleanMem();*/
		}
}

extern "C"{
	__declspec(dllexport)
		void CreategameWithHandle(HWND hWnd){
			SetHandleParent(hWnd);
			Creategame();
		}
}

HWND GetContext(){
	return esContext.hWnd;
}

extern "C"
{
	__declspec(dllexport)
		void ExecuteGame(LPCWSTR method, LPVOID param){
			
			if (wcsstr(method, (LPCWSTR)"Rungame")){
				MessageBox(esContext.hWnd, wcsstr(method, (LPCWSTR)"R"), (LPCWSTR)method, MB_OK);
				Rungame();
			}
		}
}

void main(){
	Creategame();
	Game2D* game = Game2D::GetInstance();
	Delete(game);
	Game::CleanMem();
}
#endif