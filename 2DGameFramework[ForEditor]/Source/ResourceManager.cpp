#include <algorithm>
#include "ResourceManager.h"
#include "Utils.h"
#include "DebugDefine.h"
using namespace std;


ResourceManager* ResourceManager::instance=0;

ResourceManager::ResourceManager()
{
	mListTexture.empty();
	mListMesh2D.empty();
	mListShader.empty();
	mListVertexPath.empty();
	mListFragmentPath.empty();
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < mListTexture.size(); i++){
		mListTexture[i]->UnRefenrence();
		if (mListTexture[i]->GetReference() <= 0)
			Delete(mListTexture[i]);
	}
	for (int i = 0; i < mListMesh2D.size(); i++){
		mListMesh2D[i]->UnRefenrence();
		if (mListMesh2D[i]->GetReference() <= 0)
			Delete(mListMesh2D[i]);
	}
}

ResourceManager* ResourceManager::GetInstance()
{
	if (instance==0)
		instance=new ResourceManager();
	return instance;
}

int ResourceManager::Load(string path2Resource)
{
	FILE* fi;
	fi=fopen(path2Resource.c_str(),"r");
	if (fi==NULL)
		return -1;
	char buffer[MAX_LEN];

	//Pass line //Texture
	ReadLine(fi, buffer);
	while (strstr(buffer,"Mesh")==0)
	{
		ReadLine(fi, buffer);
		if (strstr(buffer,"Mesh")!=0)
			break;
		char path[MAX_LEN];
		memset(path,0,MAX_LEN);
		int iPos=0;
		iPos=ScanPath(buffer,path,iPos);
		int referenceCount=0;
		iPos=ScanNumberInt32(buffer,&referenceCount,iPos);
		if (path!=0)
		{
			Texture* texture=new Texture();
			texture->Init(path,referenceCount);
			mListTexture.push_back(texture);
		}
	}
	//Pass line //Shader
	ReadLine(fi,buffer);
	//Pass line //VertextShader
	ReadLine(fi, buffer);
	while (strstr(buffer, "FragmentShaderList") == 0)
	{
		ReadLine(fi, buffer);
		if (strstr(buffer, "FragmentShaderList") != 0)
			break;
		char pathVertexShader[MAX_LEN];
		memset(pathVertexShader, 0, MAX_LEN);
		int iPos = 0;
		iPos = ScanPath(buffer, pathVertexShader, iPos);
		mListVertexPath.push_back(string(pathVertexShader));
	}
	//Pass line //FragmentShaderList
	ReadLine(fi, buffer);
	while (strstr(buffer, "End") == 0)
	{
		ReadLine(fi, buffer);
		if (strstr(buffer, "End") != 0)
			break;
		char pathVertexShader[MAX_LEN];
		memset(pathVertexShader, 0, MAX_LEN);
		int iPos = 0;
		iPos = ScanPath(buffer, pathVertexShader, iPos);
		mListVertexPath.push_back(string(pathVertexShader));
	}
	return 0;
}

bool IsExistPath(char *path1,char *path2)
{
	return path1==path2;
}

Texture* ResourceManager::GetTexture(char* path)
{
	Texture* res=NULL;
	int n=mListTexture.size();
	if (n==0)
		return res;
	vector<Texture*>::iterator tex=find_if(mListTexture.begin(),mListTexture.end(),MatchStringTexture(string(path)));
	if (tex!=mListTexture.end())
	{
		auto index = distance(mListTexture.begin(), tex);
		return mListTexture.at(index);
	}
	return res;
}

Shaders* ResourceManager::GetShader(char* pathVertext,char* pathFragment)
{
	vector<Shaders*>::iterator tex = find_if(mListShader.begin(), mListShader.end(), MatchStringShader(string(pathVertext),string(pathFragment)));
	if (tex!=mListShader.end())
	{
		auto index = distance(mListShader.begin(), tex);
		return mListShader.at(index);
	}
	else{
		Shaders* sha = new Shaders();
		sha->Init(pathVertext, pathFragment, 0);
		mListShader.push_back(sha);
		return sha;
	}
}