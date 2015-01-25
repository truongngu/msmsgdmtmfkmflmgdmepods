#include "Texture.h"
#include "DebugDefine.h"
#include "Global.h"
//#include "SOIL.h"
#include "Shader.h"

Texture::~Texture()
{
	glDeleteTextures(1,&mTextureid);
}

void Texture::Init(string path2Img,int referenceCount)
{
	mPath2Img=path2Img;
	mReferenceCount=referenceCount;
}

void Texture::Load(TextureType type)
{
	switch (type)
	{
	case Texture2D:
		Load2DTexture();
		break;
	case TextureCube:
		break;
	case Texture3D:
		break;
	default:
		break;
	}
	isLoaded=true;
}

void Texture::Load2DTexture()
{
	if (mPath2Img.c_str()==0)
		return;
	//Texture info
	int iWidth, iHeight, iBPP;
	char* imageData = NULL;
	GLenum eTextureFormat;

	char path[MAX_LEN];
	strcpy(path,Global::gameResourceDir.c_str());
	strcat(path,mPath2Img.c_str());
	glActiveTexture(GL_TEXTURE0);
	
	if (strstr(path,".tga")!=0)
	{
		imageData = LoadTGA(path, &iWidth, &iHeight, &iBPP);
		switch (iBPP)
		{
			case 24:
				eTextureFormat = GL_RGB;
				break;
			case 32:
				eTextureFormat = GL_RGBA;
				break;
			default:
				LogOut("Error Load Texture!!! Path:");
				LogOut(mPath2Img.c_str());
				break;
		}
		glGenTextures(1,&mTextureid);
		glBindTexture(GL_TEXTURE_2D, mTextureid);
		glTexImage2D(GL_TEXTURE_2D, 0, eTextureFormat, iWidth, iHeight, 0, eTextureFormat, GL_UNSIGNED_BYTE, imageData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		Delete_Array(imageData);
	}
	/*else if (strstr(path,".png"))
	{
		mTextureid=SOIL_load_OGL_texture(path,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		glBindTexture(GL_TEXTURE_2D, mTextureid);
	}*/
	

	/*glTexImage2D(GL_TEXTURE_2D, 0, eTextureFormat, iWidth, iHeight, 0, eTextureFormat, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mTextureType=TextureType::Texture2D;
}

void Texture::Render(Shaders* shader)
{
	if (mTextureid==-1||mPath2Img.c_str()==0)
		return;
	switch (mTextureType)
	{
	case TextureType::Texture2D:
		{
			for (int i=0;i<10;i++)
			{
				if (shader->samples[i] >-1)
				{
					glActiveTexture(GL_TEXTURE0+i);
					glBindTexture(GL_TEXTURE_2D, mTextureid);
					glUniform1i(shader->samples[i], i);
				}
			}
		}
	break;
	case TextureType::TextureCube:
	break;
	default:
		break;
	}
}

void Texture::Unrender()
{
	if (mTextureid==-1||mPath2Img.c_str()==0)
		return;
	switch (mTextureType)
	{
	case Texture2D:
		  for (int i=0;i<10;i++)
			{
					glActiveTexture(GL_TEXTURE0+i);
					glBindTexture(GL_TEXTURE_2D, 0);
			}
		break;
	case TextureCube:
		break;
	case Texture3D:
		break;
	default:
		break;
	}
}