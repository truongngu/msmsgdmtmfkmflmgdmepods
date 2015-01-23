#include "DebugDefine.h"

#include "SoundManager.h"
//
//#ifdef Android
//extern "C" {
//    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
//    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
//};
//
//JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
//{
//	Game* mainGame = Game::GetInstance();
//	if (mainGame->CreateGame() == 0)
//	{
//		delete Globals::currentCamera;
//		Delete(mainGame);
//		printf("%s", "Error!");
//		_getch();
//		return 0;
//	}
//}
//
//JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
//{
//	Game* mainGame = Game::GetInstance();
//    mainGame->Draw(0.0f);
//}
//
//void onTouch(int id, float x, float y)
//{
//	/*Game* mainGame = Game::GetInstance();
//	MouseData mouseData;
//	mouseData.type = MouseType::NotClicked;
//	mouseData.isDown = false;
//	switch (id)
//	{
//	case Action::Up:
//		mouseData.type = MouseType::Left;
//		mouseData.position = Vector2(x, y);
//		mouseData.isDown = false;
//		break;
//	case Action::Down:
//		mouseData.type = MouseType::Left;
//		mouseData.position = Vector2(x, y);
//		mouseData.isDown = true;
//	break;
//	case Action::Move:
//		mouseData.position = Vector2(x, y);
//	break;
//	default:
//		break;
//	}
//	mainGame->Mouse(mouseData,mouseData.isDown);*/
//}
//
//#endif
//
//
//
//#ifdef Win32
//int _tmain(int argc, char* argv[])
//{
//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
//	ESContext esContext;
//	Game* game=Game::GetInstance();
//	if (game->CreateGame(&esContext)!=0)
//	{
//		LogOut("Error while creating game!");
//		Delete(game);
//		return -1;
//	}
//	game->StartGame(&esContext);
//	Delete(game);
//	return 0;
//
//}
//
//#endif

//void main()
//{
//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
//
//	SoundManager* s=SoundManager::getInstance();
//	s->LoadOGG("test.ogg");
//	
//	s->PlayMusic("test.ogg",true);
//
//	//SoundSystem::Initialise();
//	//Sound::AddSound("test.ogg");
//	//Sound::AddSound("rom.ogg");
//	////SoundPlayer*sound=new SoundPlayer(Sound::GetSound("test.ogg"));
//	//SoundSystem* s=SoundSystem::GetSoundSystem();
//	////s->AddSoundPlayer(sound);
//	//s->PlaySound(Sound::GetSound("test.ogg"),SoundPriority::SOUNDPRIORITY_ALWAYS,true);
//	bool play=false;
//	
//	float t=0;
//	do
//	{
//		t+=0.01f;
//		s->PlayMusic("test.ogg",true);
//		if (t>1000.0f && !play)
//		{
//			s->StopAll();
//			play=true;
//		}
//	}while(t<12000.0f);
//	Delete(s);
//}