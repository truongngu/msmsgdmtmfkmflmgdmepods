#include "MathEx.h"
enum MouseType
{
	NotClicked,
	Left,
	Middle,
	Right
};
struct MouseData
{
	MouseType type;
	Vector2 position;
	bool isDown;

	MouseData() :type(NotClicked), position(0, 0), isDown(false){}
};