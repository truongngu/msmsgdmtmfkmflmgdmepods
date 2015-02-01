// if you use STL, please include this header AFTER all other include
#pragma once

#include "DebugDefine.h"
#if defined Win32
#include "esUtil.h"
#include "esUtil_win.h"
#endif
#if defined WindowPhone
typedef unsigned int     GLuint;
typedef unsigned int     GLenum;
typedef int              GLint;
#endif
#include "Math.h"
#include "MathEx.h"
#include "TGA.h"
#include "memDbg.h"
#include "MouseData.h"
