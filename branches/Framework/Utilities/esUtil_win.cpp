#include "DebugDefine.h"
#if defined Win32
#include "stdafx.h"
#include <windows.h>
#include <CommCtrl.h>
#include "esUtil.h"


HWND esGL = 0;
ESContext *windownEsContext;
HWND esGLparent = 0;
bool isNative = false;
int esGLparentX, esGLparentY, esGLparentW, esGLparentH;
DWORD lastTime = GetTickCount();
void SetEGLWindowParent(HWND parent){
	esGLparent = parent;
	isNative = true;
}

void SetEGLWindowLocation(int x, int y, int wid, int hei){
	esGLparentX = x;
	esGLparentY = y;
	esGLparentW = wid;
	esGLparentH = hei;
}

void ExecuteEGLWindow(char* method, void* param){
	if (strstr(method, "UpdateGame")){

	}
}
// Main window procedure
#if defined InNative
LRESULT WINAPI ESWindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	LRESULT  lRet = 1; 

	MouseData mouseData;
	mouseData.type = MouseType::NotClicked;
	mouseData.isDown = false;
	ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
	switch (uMsg) 
	{ 
	case WM_CREATE:
		break;

	case WM_PAINT:
	{

					 if ( esContext && esContext->drawFunc )
						 esContext->drawFunc ( esContext );

					 ValidateRect( esContext->hWnd, NULL );
	}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);             
		break; 

	case WM_KEYDOWN:
	{

					   if ( esContext && esContext->keyFunc )
						   esContext->keyFunc ( esContext, (unsigned char) wParam, true );
	}
		break;

	case WM_KEYUP:
	{

					 if ( esContext && esContext->keyFunc )
						 esContext->keyFunc ( esContext, (unsigned char) wParam, false );
	}
		break;
	case  WM_LBUTTONDOWN:
	{
							POINT p;
							GetCursorPos(&p);
							ScreenToClient(hWnd, &p);
							mouseData.type = MouseType::Left;
							mouseData.position = Vector2(p.x, p.y);
							mouseData.isDown = true;
							if (esContext && esContext->keyFunc)
								esContext->mouseFunc(esContext, mouseData, true);
	}
		break;
	case  WM_LBUTTONUP:
	{
						  POINT p;
						  GetCursorPos(&p);
						  ScreenToClient(hWnd, &p);
						  mouseData.type = MouseType::Left;
						  mouseData.position = Vector2(p.x, p.y);
						  mouseData.isDown = false;
						  if (esContext && esContext->keyFunc)
							  esContext->mouseFunc(esContext, mouseData, false);
	}
		break;
	case  WM_RBUTTONDOWN:
	{
							POINT p;
							GetCursorPos(&p);
							ScreenToClient(hWnd, &p);
							mouseData.type = MouseType::Right;
							mouseData.position = Vector2(p.x, p.y);
							mouseData.isDown = true;
							if (esContext && esContext->keyFunc)
								esContext->mouseFunc(esContext, mouseData, true);
	}
		break;
	case  WM_RBUTTONUP:
	{
						  POINT p;
						  GetCursorPos(&p);
						  ScreenToClient(hWnd, &p);
						  mouseData.type = MouseType::Right;
						  mouseData.position = Vector2(p.x, p.y);
						  mouseData.isDown = false;
						  if (esContext && esContext->keyFunc)
							  esContext->mouseFunc(esContext, mouseData, false);
	}
		break;
	case  WM_MOUSEMOVE:
	{
						  POINT p;
						  GetCursorPos(&p);
						  ScreenToClient(hWnd, &p);
						  mouseData.position = Vector2(p.x, p.y);
						  if (esContext && esContext->keyFunc)
							  esContext->mouseFunc(esContext, mouseData, false);
	}
		break;

	default: 
		lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}

//      Create Win32 instance and window
GLboolean WinCreate ( ESContext *esContext, const char *title )
{
	WNDCLASS wndclass = {0}; 
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC)ESWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = "opengles2.0"; 

	if (!RegisterClass (&wndclass) ) 
		return FALSE; 

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect ( &windowRect, wStyle, FALSE );

	esContext->hWnd = CreateWindow(
		"opengles2.0",
		title,
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the 
	// ESWindowProc
	SetWindowLongPtr (  esContext->hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext );

	if ( esContext->hWnd == NULL )
		return GL_FALSE;

	ShowWindow ( esContext->hWnd, TRUE );

	return GL_TRUE;
}
#endif


//      Start main windows loop
void WinLoop ( ESContext *esContext )
{
#if defined InNative
	MSG msg = { 0 };
	int done = 0;
	DWORD lastTime = GetTickCount();

	while (!done)
	{
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		DWORD curTime = GetTickCount();
		float deltaTime = (float)( curTime - lastTime ) / 1000.0f;
		lastTime = curTime;

		if ( gotMsg )
		{
			if (msg.message==WM_QUIT)
			{
				done=1; 
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		else
			SendMessage( esContext->hWnd, WM_PAINT, 0, 0 );

		// Call update function if registered
		if ( esContext->updateFunc != NULL )
			esContext->updateFunc ( esContext, deltaTime );
	}
#endif
}

HCURSOR _cursor;
void HIDE_CURSOR()
{
	_cursor = GetCursor();
	SetCursor(NULL);
}

void SHOW_CURSOR()
{
	SetCursor(_cursor);
}

GLboolean WinCreate(ESContext *esContext, const char *title)
{
	windownEsContext = esContext;
	
	DWORD wStyle = WS_VISIBLE | WS_CHILD;

	esContext->hWnd = CreateWindow(
		WC_STATIC,
		title,
		wStyle,
		esGLparentX,
		esGLparentY,
		esGLparentW,
		esGLparentH,
		esGLparent,
		NULL,
		NULL,
		NULL);
	
	esGL = esContext->hWnd;
	if (esContext->hWnd == NULL)
		return GL_FALSE;

	return GL_TRUE;
}
#endif