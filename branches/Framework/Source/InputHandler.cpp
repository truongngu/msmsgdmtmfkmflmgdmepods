#include "InputHandler.h"
#include "DebugDefine.h"

#if defined Win32 || defined WindowStore
bool InputHandler::KeyDown[255];
bool InputHandler::KeyUp[255];
MouseData InputHandler::mouseData;
int InputHandler::prevKey=1000;
int InputHandler::currentKey=1000;

void InputHandler::Init()
{
	memset(KeyDown,0,sizeof(KeyDown));
	memset(KeyUp,1,sizeof(KeyUp));
}
void InputHandler::SetKeyDown(int key)
{
	KeyUp[key]=false;
	KeyDown[key]=true;
	prevKey=currentKey;
	currentKey=key;
}

void InputHandler::SetKeyUp(int key)
{
	KeyDown[key]=false;
	KeyUp[key]=true;
	prevKey=key;
	//currentKey=1000;
}

bool InputHandler::IsKeyDown(int key)
{
	if (KeyDown[key] /*&& currentKey==key*/)
	{
		//currentKey=1000;
		return true;
	}
	return false;
}

bool InputHandler::IsKeyUp(int key)
{

	if (KeyUp[key] && prevKey==key){
		prevKey=1000;
		return true;
	}
	return false;
}

void InputHandler::SetMouseData(MouseData m)
{
	mouseData=m;
}
#endif