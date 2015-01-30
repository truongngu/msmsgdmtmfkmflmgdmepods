#pragma once
#include "2DGameFramework\Sprite.h"


class Bird:public Sprite
{
public:
	Bound GetBound()
	{
		Bound b=Sprite::GetBound();
		b.topright.x=b.bottomright.x=b.bottomright.x-0.4f;
		b.topleft.y=b.topright.y=b.topleft.y-0.3f;
		b.bottomleft.y=b.bottomright.y=b.bottomleft.y+0.3f;
		if (mRotateAngles.z>0.0f)
			b.topleft.y=b.topright.y=b.topleft.y+0.3f;
		return b;
	}
	Bird* Create()
	{
		return new Bird();
	}

	Bird* Clone()
	{
		Bird* bird=new Bird();
		CloneAttribute(bird);
		return bird;
	}
};