#pragma once
#include <time.h>

class FPSLimitter
{
private:
	float mLimitFPS;
	float beginSec;
	float endSec;
	float totalSec;
	FPSLimitter();
	static FPSLimitter* instance;
protected:
public:
	static FPSLimitter* GetInstance();
	void SetFPSLimitValue(float value);
	float GetFPSLimitValue();
	void BeginFPS();
	void EndFPS();
	static void Destroy();
};