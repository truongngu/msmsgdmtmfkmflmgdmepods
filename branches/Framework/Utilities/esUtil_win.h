#pragma once
#include "DebugDefine.h"
#if defined Win32
// Create Win32 instance and window
GLboolean WinCreate ( ESContext *esContext, const char *title );



// Start main windows loop
void WinLoop ( ESContext *esContext );



void HIDE_CURSOR();

void SHOW_CURSOR();
#endif
