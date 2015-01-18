#pragma once
#include <string>
#include "../Utilities/utilities.h"
#include "Resource.h"
#include "Shader.h"
using namespace std;


enum TextureType
{
	Texture2D=0,
	TextureCube,
	Texture3D
};

class Texture:public Resource
{
private:
	GLuint mTextureid;
	string mPath2Img;	//For debug only
	TextureType mTextureType;
	bool isLoaded;

	void Load2DTexture();
	void Load3DTexture();
	void LoadCubeTexture();
protected:
public:
	Texture(){mTextureid=-1;isLoaded=false;}
	~Texture();
	string GetPath(){return mPath2Img;}
	string GetType(){return string("Texture");}
	bool IsLoaded(){return isLoaded;}
	void Init(string path2Img,int referenceCount);
	void Load(TextureType type);
	void Render(Shaders* sha);
	void Unrender();
};