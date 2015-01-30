#pragma once
#include <stdio.h>
#include <Windows.h>
#include "2DGameFramework\DebugDefine.h"
#include "2DGameFramework\State.h"
#include "2DGameFramework\StateManager.h"
#include "2DGameFramework\Utils.h"
#include "2DGameFramework\ConvertUtils.h"
#include "Utilities\esUtil_win.h"
#include "Game2D.h"

using namespace System;

namespace Game2DDemo{
	ESContext esContext;
	public ref class MyResult{
		float x;
		float y;
		String^ name;
		int result;
	public:
		MyResult();
		MyResult(float x);
		MyResult(float x, float y);
		~MyResult(){}
		float GetX();
		void SetX(float X);
		float GetY();
		void SetY(float Y);
		void SetName(std::string Name);
		String^ GetName();
		int GetResult();
		void Result(int Result);
	};
	public ref class GameInstance{
		
	public:
		GameInstance();
		~GameInstance();
		void Creategame();
		void Rungame(float deltaTime);
		void ExitRequestHandle();
		void SetHandleParent(HWND parent);
		void SetWindowSizeChange(int x, int y, int w, int h);
		void SetHandleLocaltion(int x, int y, int w, int h);
		void CreategameWithHandle(void* hWnd);
		void KeyDownRequestHandle(unsigned char key);
		void KeyUpRequestHandle(unsigned char key);
		void MouseRequestHandle(bool isDown, int x, int y, int type);
		void MoveCameraTo(float x, float y);
		void MoveSelectedEntity(float deltax, float deltay);
		int MoveTheSelectedEntity(String^ name, float* eX, float* eY, float deltax, float deltay);
		MyResult^ MoveTheSelectedEntity(float deltax, float deltay);
		int GetPickingEntity(String^ name, float x, float y);
		MyResult^ GetPickingEntity(float x, float y);
		void ReleaseSelectedEntity();
		void* GetContextHandle();
		void ExecuteGame(LPCWSTR method, LPVOID param);
	};
}
