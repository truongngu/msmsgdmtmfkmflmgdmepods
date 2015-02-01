#pragma once
#include "DebugDefine.h"
#if defined WindowPhone
#include "Direct3DBase.h"

// This class renders a simple spinning cube.
class MyRenderer sealed : public Direct3DBase
{
public:
	MyRenderer();

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override; //init d3d
	virtual void CreateWindowSizeDependentResources() override; //init d3d
	virtual void Render(float deltaTime) override;
	void Update(float timeTotal, float timeDelta);
private: 
	float tz;
	float dz;
};
#endif