#pragma once
#include <string>
using namespace std;
//
//enum ResourceType
//{
//	Unknow,
//	Texture,
//	Shader,
//	Mesh
//};

class Resource
{
protected:
	int mReferenceCount;
	//ResourceType type;
public:
	Resource(){//type=Unknow;
		mReferenceCount=0;
	}
	void Refenrence();
	void UnRefenrence();
	int GetReference();
	virtual string GetType()=0;
};