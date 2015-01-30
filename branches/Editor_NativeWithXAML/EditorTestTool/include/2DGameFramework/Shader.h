#pragma once
#include <string>
#include "DebugDefine.h"
#include "Resource.h"
#include "../Utilities/utilities.h"
using namespace std;

class Shaders:public Resource
{
private:
	string mName;
	char* mfileVertextShader;
	char* mfileFragmentShader;
	bool isLoaded;
public:
	Shaders(){ mReferenceCount = 0; isLoaded = false; mName = ""; }
	~Shaders();
	string GetName(){
		return mName;
	}
	void SetName(string value){
		mName = value;
	}
	bool IsLoaded(){return isLoaded;}
	string GetType(){return string("Shader");}
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint a_positon, a_uv, a_normal, a_binormal, a_tangent, a_color, u_World, u_MatrixMVP, u_Camera;
	GLuint u_Color;
	GLuint u_Tran;
	GLuint u_IsProgress;

	GLint u_texture;
	GLint timeUniform;
	GLint cubeSampler;
	GLint uniformCameraPosition;
	GLint uniformObjectWorld;
	GLint uniformLightPosition;
	GLint uniformLightColor;//Diffcolor
	GLint uniformLightAmbient;
	GLint uniformLightSpecular;
	GLint uniformLightDirect;//for spot light
	GLint uniformTankBoost;
	//For Text Draw 
	GLint uniformColorText;
	//For sprite
	GLfloat u_dx;
	GLfloat u_dy;

	//Control Attenuation
	GLint uniformA0;
	GLint uniformA1;
	GLint uniformA2;
	
	GLint dirtSampler;
	GLint rockSampler;
	GLint grassSampler;
	GLint blendSampler;

	GLint samples[10];
	void Init(char * fileVertexShader, char * fileFragmentShader, int referenceCount);
	int Load();
	string GetVertexShaderPath(){return string(mfileVertextShader);}
	string GetFragmentShaderPath(){ return string(mfileFragmentShader); }
};