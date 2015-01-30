#pragma once
#include "2DGameFramework\Game.h"
class Game2D:public Game
{
private:
	static Game2D* instance;
	Game2D(){isInit=false;}
public:
	static Game2D* GetInstance();
#ifdef Win32
	int CreateGame(ESContext*);
#endif
#ifdef Android
	int CreateGame();
#endif
#ifdef WindowPhone
	int CreateGame();
#endif
};

