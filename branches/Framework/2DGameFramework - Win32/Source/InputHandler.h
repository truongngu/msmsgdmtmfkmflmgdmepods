#pragma once
#include "../Utilities/utilities.h"
#include "DebugDefine.h"

enum Action
{
	DOWN,
	UP,
	MOVE,
	CANCEL,
	OUT_SIDE,
	POINTER_DOWN,
	POINTER_UP,
	HOVER_MOVE,
	HOVER_ENTER,
	HOVER_EXIT,
	NONE
};

class InputHandler
{
#ifdef Win32
private:
	static bool KeyDown[255];
	static bool KeyUp[255];
	static MouseData mouseData;
	static int prevKey;
	static int currentKey;
public:
	static void Init();
	static void SetKeyDown(int key);
	static void SetKeyUp(int key);
	static bool IsKeyDown(int key);
	static bool IsKeyUp(int key);
	static void SetMouseData(MouseData);
	static bool IsMouseDown();
	static bool IsMouseUp();
	static MouseData GetMouseData();
#endif
};
