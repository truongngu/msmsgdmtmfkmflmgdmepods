#include "FPSLimiter.h"
#include "DebugDefine.h"
#include "Utils.h"
#ifdef Android
#include <unistd.h>
#endif

FPSLimitter* FPSLimitter::instance = 0;

FPSLimitter* FPSLimitter::GetInstance()
{
	if (instance == 0)
		instance = new FPSLimitter();
	return instance;
}

FPSLimitter::FPSLimitter()
{
	mLimitFPS = 60.0f;
}

void FPSLimitter::BeginFPS()
{
	beginSec = GetTimeNow();
}

void FPSLimitter::EndFPS()
{
	endSec = GetTimeNow();
	totalSec = endSec - beginSec;
	float limitTime = 1.0f / mLimitFPS*1000.0f;
	if (totalSec < limitTime)
	{
		SleepForATime(limitTime - totalSec);
	}
}

void FPSLimitter::Destroy()
{
	Delete(instance);
}

void FPSLimitter::SetFPSLimitValue(float value)
{
	mLimitFPS = value;
}

float FPSLimitter::GetFPSLimitValue(){
	return mLimitFPS;
}