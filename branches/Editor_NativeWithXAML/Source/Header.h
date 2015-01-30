#pragma once
#include "2DGameFramework\Sprite.h"


class Bird:public Sprite
{
	Bound GetBound()
	{
		Bound b=Sprite::GetBound();
	}
};