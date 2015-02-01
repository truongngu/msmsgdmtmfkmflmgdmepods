#include "Global.h"

BaseCamera* Global::currentCamera=new Camera();
#if defined Win32 || defined Android
string Global::Path2Resource = "Resources/Res.txt";
string Global::gameResourceDir = "../";
string Global::gameResourcePath = "../Resources/";
#endif
#ifdef WindowPhone
string Global::Path2Resource = "Resources/Res.txt";
string Global::gameResourceDir = "Assets/";
string Global::gameResourcePath = "Assets/Resources/";
#endif
Shaders* Global::BoundShader=new Shaders();
Shaders* Global::TextShader=new Shaders();
Shaders* Global::TextShaderInWorld = new Shaders();
int Global::ScreenWidth=1076;
int Global::ScreenHeight=720;
#ifdef WindowPhone
ID3D11Device1* Global::m_d3ddevice = nullptr;
#endif
b2World* Global::gWorld = new b2World(b2Vec2(0.0f, -10.0f));

