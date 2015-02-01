#pragma once
#include <vector>
#include "Texture.h"
#include "Mesh2D.h"
using namespace std;

class ResourceManager
{
private:
	ResourceManager();
	static ResourceManager* instance;
	vector<Texture*> mListTexture;
	vector<Mesh2D*> mListMesh2D;
	vector<Shaders*> mListShader;
	vector<string> mListVertexPath;
	vector<string> mListFragmentPath;
protected:
public:
	~ResourceManager();
	static ResourceManager* GetInstance();
	int Load(string path2ResourceDef);
	Texture* GetTexture(char* path2Img);
	Shaders* GetShader(char*path2VertexShader,char* path2FragmentShader); 
};

