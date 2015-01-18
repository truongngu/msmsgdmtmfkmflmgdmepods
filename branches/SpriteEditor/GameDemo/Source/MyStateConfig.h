#pragma once
#include "2DGameFramework\StateConfig.h"

class MyStateConfig : public StateConfig{
public:
	MyStateConfig();
	~MyStateConfig();
	virtual void ReadConfig(const char*);
};