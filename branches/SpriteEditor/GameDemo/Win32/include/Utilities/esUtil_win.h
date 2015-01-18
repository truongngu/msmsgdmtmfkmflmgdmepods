#pragma once


// Create Win32 instance and window
GLboolean WinCreate ( ESContext *esContext, const char *title );

void SetEGLWindowParent(HWND parent);
void SetEGLWindowLocation(int x, int y, int wid, int hei);
void ChangeEGLWindowSize(int x, int y, int wid, int hei);
void ExecuteEGLWindow(char* method, void* param);
// Start main windows loop
void WinLoop ( ESContext *esContext );

void HIDE_CURSOR();

void SHOW_CURSOR();
