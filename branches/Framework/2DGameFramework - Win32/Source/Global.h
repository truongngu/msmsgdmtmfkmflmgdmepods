#pragma once
#include <string>
#include "BaseCamera.h"
#include "Camera.h"
#include "Shader.h"
#include "Box2D.h"

using namespace std;

class Global
{
public:
	static BaseCamera* currentCamera;
	static int ScreenWidth;
	static int ScreenHeight;
	static string Path2Resource;
	static string gameResourceDir;
	static string gameResourcePath;
	static Shaders* BoundShader;
#ifdef WindowPhone
	static ID3D11Device1* m_d3ddevice;
#endif
	static Shaders* TextShader;
	static Shaders* TextShaderInWorld;
	static b2World* gWorld;
};


