#include "Texture.h"
#include "DebugDefine.h"
#include "Global.h"
//#include "SOIL.h"
#include "Shader.h"
#ifdef WindowPhone
#include "Global.h"
#include <mutex>
#include "DirectXHelper.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
#endif

Texture::~Texture()
{
#ifdef Win32
	glDeleteTextures(1, &mTextureid);
#endif
}

void Texture::Init(string path2Img, int referenceCount)
{
	mPath2Img = path2Img;
	mReferenceCount = referenceCount;
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
	isLoaded = true;
}

void Texture::Load2DTexture()
{
	if (mPath2Img.c_str() == 0)
		return;
	//Texture info
	int iWidth, iHeight, iBPP;
	char* imageData = NULL;
	GLenum eTextureFormat;

	char path[MAX_LEN];
	strcpy(path, Global::gameResourceDir.c_str());
	strcat(path, mPath2Img.c_str());
#if defined Win32 || defined Android
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
#endif
#if defined WindowPhone
	if (strstr(path, ".tga") != 0)
	{
		int pixelSize = 3;
		imageData = LoadTGA(path, &iWidth, &iHeight, &iBPP);
		switch (iBPP)
		{
		case 24:
			eTextureFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case 32:
			pixelSize = 4;
			eTextureFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			LogOut("Error Load Texture!!! Path:");
			LogOut(mPath2Img.c_str());
			break;
		}

		std::mutex  m_mutex;
		std::lock_guard<std::mutex> lock(m_mutex);
		CD3D11_TEXTURE2D_DESC textureDesc = CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			static_cast<UINT>(iWidth),
			static_cast<UINT>(iHeight),
			1,
			1,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE_DYNAMIC,
			D3D11_CPU_ACCESS_WRITE,
			1,
			0,
			0
			);

		//int len = strlen(buffer);
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = imageData;
		data.SysMemPitch = pixelSize * iWidth;
		//data.SysMemSlicePitch = pixelSize * width * height;

		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreateTexture2D(
			&textureDesc,
			&data,
			&m_Texture
			)
			);


		Global::m_d3ddevice->CreateShaderResourceView(m_Texture.Get(), NULL, &SRV);
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		Global::m_d3ddevice->CreateSamplerState(&sampDesc, &TexSamplerState);

		Delete_Array(imageData);

		mTextureType = TextureType::Texture2D;
	}
#endif
}

void Texture::Render(Shaders* shader)
{
#if defined Win32 || defined Android
	if (mTextureid == -1 || mPath2Img.c_str() == 0)
		return;
#endif
#if defined WindowPhone
	const char* path = mPath2Img.c_str();
	if (mPath2Img.c_str() == 0)
		return;
#endif
	switch (mTextureType)
	{
	case TextureType::Texture2D:
	{
#if defined Win32 || defined Android
		for (int i = 0; i<10; i++)
		{
			if (shader->samples[i] >-1)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mTextureid);
				glUniform1i(shader->samples[i], i);
			}
		}
#endif
#if defined WindowPhone
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
		Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);

		m_d3dContext->PSSetShaderResources(0, 1, SRV.GetAddressOf());
		m_d3dContext->PSSetSamplers(0, 1, TexSamplerState.GetAddressOf());
#endif
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
	if (mTextureid == -1 || mPath2Img.c_str() == 0)
		return;
#if defined WindowPhone
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);
	Microsoft::WRL::ComPtr<ID3D11Resource> resourse;
#endif
	switch (mTextureType)
	{
	case Texture2D:
#if defined Win32 || defined Android
		for (int i = 0; i<10; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
#endif
#if defined WindowPhone
		SRV.Get()->GetResource(resourse.GetAddressOf());
		m_d3dContext->PSSetShaderResources(0, 1, 0);
		m_d3dContext->DiscardResource(resourse.Get());
		m_d3dContext->PSSetSamplers(0, 1, 0);
		m_d3dContext->Unmap(resourse.Get(), 0);
#endif
		break;
	case TextureCube:
		break;
	case Texture3D:
		break;
	default:
		break;
	}
}