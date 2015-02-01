#pragma once
#include <vector>
#include "../Utilities/utilities.h"
#include "DebugDefine.h"
#include "Global.h"
using namespace std;

class Game
{
private:
	
protected:
	static Game* instance;
	bool isActive;
	bool isInit;
	Game();
public:
	static vector<bool> KeyDown;
	static vector<string> TouchDown;
	~Game();
	static void CleanMem();
	static Game* GetInstance();
	
#if defined Win32
	virtual int CreateGame(ESContext*);
	virtual void StartGame(ESContext*);
	static void Mouse(ESContext *esContext,MouseData mouse, bool bIsDown);
	static void Key(ESContext *esContext,unsigned char key, bool bIsPressed);
	static void DrawFrame(ESContext *esContext,float deltaTime);
#endif
#if defined WindowPhone || defined Android
	int CreateGame();
	void StartGame();
	static void onTouch(int id, float x, float y);
	static void Key(unsigned char key, bool bIsPressed);
	static void DrawFrame(float deltaTime);
#endif
	void Pause();
	void Resume();
	bool IsInit();
};