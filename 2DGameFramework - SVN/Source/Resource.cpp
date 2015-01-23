#include "Resource.h"

void Resource::Refenrence()
{
	mReferenceCount++;
}
void Resource::UnRefenrence()
{
	mReferenceCount--;
}
int Resource::GetReference()
{
	return mReferenceCount;
}