#pragma once
#include "Entity2D.h"

class StateMachine
{
public:
	virtual char* GetName()=0;
	virtual void Action(Entity2D*)=0;
};