#include "Global.h"

BaseCamera* Global::currentCamera=new Camera();
string Global::Path2Resource="Resources/Res.txt";
Shaders* Global::BoundShader=new Shaders();
Shaders* Global::TextShader=new Shaders();
Shaders* Global::TextShaderInWorld = new Shaders();
string Global::gameResourceDir="../";
string Global::gameResourcePath="../Resources/";
int Global::ScreenWidth=1076;
int Global::ScreenHeight=720;
b2World* Global::gWorld = new b2World(b2Vec2(0.0f, -10.0f));

