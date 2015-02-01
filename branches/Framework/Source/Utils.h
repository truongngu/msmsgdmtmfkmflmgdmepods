#pragma once
#include <stdio.h>
#include "Texture.h"
#include "Shader.h"
#include "Entity2D.h"
#include "State.h"
#define MAX_VAL 100000
#define MIN_VAL -100000
#define RANDOMSEEDX rand()*21231215
#define RANDOMSEEDY rand()*79841325
#define RANDOMSEEDZ rand()*32496511

struct MatchStringTexture
{
 MatchStringTexture(const std::string& s) : s_(s) {}
 bool operator()(Texture* obj) 
 {
	 string tam=obj->GetPath();
	 string s=s_;
	 return obj->GetPath() == s_;

 }
 private:
   const std::string& s_;
};

struct MatchStringShader
{
 MatchStringShader(const std::string& v,const std::string& f) : v_(v),f_(f) {}
 bool operator()(Shaders* obj) 
 {
	 string vertext=obj->GetVertexShaderPath();
	 string fragment = obj->GetFragmentShaderPath();
	 string v=v_;
	 string f = f_;
	 return (vertext == v && fragment==f);
 }
 private:
   const std::string& v_;
   const std::string& f_;
};


struct MatchStringState
{
 MatchStringState(const std::string& s) : s_(s) {}
 bool operator()(State* obj) 
 {
	 return obj->GetName() == s_;
 }
 private:
   const std::string& s_;
};

struct CompareEntity2DByZ{
	bool operator()(Entity2D* lx, Entity2D* rx) const {
		return lx->GetPosition().z > rx->GetPosition().z;
	}
};

void ReadLine(FILE *pFile, char* buffer);

bool ReadToKey(FILE* pf, char key);

int SeekToNumber(char* buffer, int iStart);

int ScanNumberInt32(char* buffer, int *iNumber, int iStart);

int ScanNumberUInt32(char* buffer, unsigned int *iNumber, int iStart);

int ScanNumberFloat32(char* buffer, float *iNumber, int iStart);

int SeekToQuote(char* buffer, int iStart);

int ScanPath(char* buffer, char* Path, int iStart);

bool IsInCameraView(Entity2D*);

unsigned int  GetARandomNumber();

Matrix InvertMatrix(Matrix);

double Det(Matrix m,int n);

double Distance(Vector3 pos1, Vector3 pos2);
#ifdef WindowPhone
Platform::String^ StringFromAscIIChars(char*);
WCHAR* StringToPointerString(const char *);
#endif
void Log(const char*s);
float Abs(float value);

float Roundf(float value);

int ArrCharLen(char*);

Vector3 ConvertCoordinate2D3D(BaseCamera* cam,Vector2 pos2D);

Vector3 ConvertCoordinate2D3DByZIndex(BaseCamera* cam,Vector2 pos2D,float zIndex);

Vector2 ConvertCoordinate3D2D(BaseCamera* cam,Vector3 pos3D);

float MaxF(float v1,float v2);
float MinF(float v1,float v2);

float GetTimeNow();

void SleepForATime(float ms);

bool checkIfExistInResourcesDictory(string);