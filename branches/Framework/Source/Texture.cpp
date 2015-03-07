#include "Texture.h"
#include "DebugDefine.h"
#include "Global.h"
#include "Utils.h"
#include "Shader.h"
#include "png.h"
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
#if defined Win32 || defined Android
	glDeleteTextures(1, &mTextureid);
#endif
#ifdef WindowPhone
	this->m_Texture.ReleaseAndGetAddressOf();
	this->SRV.ReleaseAndGetAddressOf();
	this->TexSamplerState.ReleaseAndGetAddressOf();
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

	FILE *fp;
	if ((fp = fopen(path, "rb")) == NULL){
		Log("Not Exist File");
		Log(path);
		return;
	}

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
	else if (strstr(path, ".png")){

		png_byte header[8];
		fread(header, 1, 8, fp);
		if (png_sig_cmp(header, 0, 8))
		{
			Log("FILE NOT SUPPORT, JUST PNG or TGA");
			Log(path);
			fclose(fp);
			return;
		}
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr)
		{
			Log("ERROR IN READ STRUCT PNG");
			Log(path);
			fclose(fp);
			return;
		}

		// create png info struct
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			Log("ERROR IN READ STRUCT PNG");
			Log(path);
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fclose(fp);
			return;
		}

		// create png info struct
		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info)
		{
			Log("ERROR IN READ STRUCT PNG");
			Log(path);
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fclose(fp);
			return;
		}

		// the code in this if statement gets called if libpng encounters an error
		if (setjmp(png_jmpbuf(png_ptr))) {
			Log("ERROR FROM LIBPNG");
			Log(path);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			return;
		}

		// init png reading
		png_init_io(png_ptr, fp);

		// let libpng know you already read the first 8 bytes
		png_set_sig_bytes(png_ptr, 8);

		// read all the info up to the image data
		png_read_info(png_ptr, info_ptr);

		// variables to pass to get info
		int bit_depth, color_type;
		png_uint_32 temp_width, temp_height;
		int width=1, height=1;
		// get info about png
		png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
			NULL, NULL, NULL);

		/*if (width){ *width = temp_width; }
		if (height){ *height = temp_height; }*/

		// Update the png info struct.
		png_read_update_info(png_ptr, info_ptr);

		// Row size in bytes.
		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		// glTexImage2d requires rows to be 4-byte aligned
		rowbytes += 3 - ((rowbytes - 1) % 4);

		// Allocate the image_data as a big block, to be given to opengl
		png_byte * image_data;
		image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
		if (image_data == NULL)
		{
			Log("COULD NOT ALLOCATE MEMORY FOR PNG IMG DATE");
			Log(path);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			return;
		}

		// row_pointers is for pointing to image_data for reading the png with libpng
		png_bytep * row_pointers = (png_bytep *)malloc(temp_height * sizeof(png_bytep));
		if (row_pointers == NULL)
		{
			fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
			Log("COULD NOT ALLOCATE MEMORY FOR PNG ROW POINTER");
			Log(path);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			free(image_data);
			fclose(fp);
			return;
		}

		// set the individual row_pointers to point at the correct offsets of image_data
		int i;
		for (i = 0; i < temp_height; i++)
		{
			row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
		}

		// read the png into image_data through row_pointers
		png_read_image(png_ptr, row_pointers);
		switch (color_type)
		{
		case 6:
			eTextureFormat = GL_RGBA;
			break;
		default:
			eTextureFormat = GL_RGB;
			break;
		}
		// Generate the OpenGL texture object
		glGenTextures(1, &mTextureid);
		glBindTexture(GL_TEXTURE_2D, mTextureid);
		glTexImage2D(GL_TEXTURE_2D, 0, eTextureFormat, temp_width, temp_height, 0, eTextureFormat, GL_UNSIGNED_BYTE, image_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		// clean up
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		free(row_pointers);
		fclose(fp);

	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mTextureType=TextureType::Texture2D;
#endif
#if defined WindowPhone
	int pixelSize = 3;
	if (strstr(path, ".tga") != 0)
	{
		imageData = LoadTGA(path, &iWidth, &iHeight, &iBPP);
		if (iBPP != 32){
			int numBit = iBPP / 8;
			//them mot kenh alpha = 1 vao cuoi RGB -> RGBA
			char * pOutBuffer = new char[iWidth * iHeight * 4];
			for (int i = 0; i < iWidth * iHeight * 3; i += 3){
				int index = i / 3 * 4;
				pOutBuffer[index] = imageData[i];
				pOutBuffer[index + 1] = imageData[i + 1];
				pOutBuffer[index + 2] = imageData[i + 2];
				pOutBuffer[index + 3] = 255;
			}

			Delete_Array(imageData);
			imageData = pOutBuffer;
		}
		switch (iBPP)
		{
		case 24:

			pixelSize = 4;
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
			(DXGI_FORMAT)eTextureFormat,
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
	else if (strstr(path, ".png")){

		png_byte header[8];
		fread(header, 1, 8, fp);
		if (png_sig_cmp(header, 0, 8))
		{
			Log("FILE NOT SUPPORT, JUST PNG or TGA");
			Log(path);
			fclose(fp);
			return;
		}
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr)
		{
			Log("ERROR IN READ STRUCT PNG");
			Log(path);
			fclose(fp);
			return;
		}

		// create png info struct
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			Log("ERROR IN READ STRUCT PNG");
			Log(path);
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fclose(fp);
			return;
		}

		// create png info struct
		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info)
		{
			Log("ERROR IN READ STRUCT PNG");
			Log(path);
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fclose(fp);
			return;
		}

		// the code in this if statement gets called if libpng encounters an error
		if (setjmp(png_jmpbuf(png_ptr))) {
			Log("ERROR FROM LIBPNG");
			Log(path);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			return;
		}

		// init png reading
		png_init_io(png_ptr, fp);

		// let libpng know you already read the first 8 bytes
		png_set_sig_bytes(png_ptr, 8);

		// read all the info up to the image data
		png_read_info(png_ptr, info_ptr);

		// variables to pass to get info
		int bit_depth, color_type;
		png_uint_32 temp_width, temp_height;
		int width = 1, height = 1;
		// get info about png
		png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
			NULL, NULL, NULL);

		/*if (width){ *width = temp_width; }
		if (height){ *height = temp_height; }*/

		// Update the png info struct.
		png_read_update_info(png_ptr, info_ptr);

		// Row size in bytes.
		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		// glTexImage2d requires rows to be 4-byte aligned
		rowbytes += 3 - ((rowbytes - 1) % 4);

		// Allocate the image_data as a big block, to be given to opengl
		png_byte * image_data;
		image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
		if (image_data == NULL)
		{
			Log("COULD NOT ALLOCATE MEMORY FOR PNG IMG DATE");
			Log(path);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			return;
		}

		// row_pointers is for pointing to image_data for reading the png with libpng
		png_bytep * row_pointers = (png_bytep *)malloc(temp_height * sizeof(png_bytep));
		if (row_pointers == NULL)
		{
			fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
			Log("COULD NOT ALLOCATE MEMORY FOR PNG ROW POINTER");
			Log(path);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			free(image_data);
			fclose(fp);
			return;
		}

		// set the individual row_pointers to point at the correct offsets of image_data
		int i;
		for (i = 0; i < temp_height; i++)
		{
			row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
		}

		// read the png into image_data through row_pointers
		png_read_image(png_ptr, row_pointers);
		switch (color_type)
		{
		case 6:
			pixelSize = 4;
			eTextureFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			pixelSize = 3;
			eTextureFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}

		std::mutex  m_mutex;
		std::lock_guard<std::mutex> lock(m_mutex);
		CD3D11_TEXTURE2D_DESC textureDesc = CD3D11_TEXTURE2D_DESC(
			(DXGI_FORMAT)eTextureFormat,
			static_cast<UINT>(temp_width),
			static_cast<UINT>(temp_height),
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
		data.pSysMem = image_data;
		data.SysMemPitch = pixelSize * temp_width;
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

		mTextureType = TextureType::Texture2D;

		// clean up
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		free(row_pointers);
		fclose(fp);

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
		for (int i = 0; i < 10; i++)
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
		/*m_d3dContext->IASetVertexBuffers(
			0,
			1,
			0,0,0
			);

		m_d3dContext->IASetIndexBuffer(
			0,
			DXGI_FORMAT_R16_UINT,
			0
			);
		m_d3dContext->VSSetShader(0, 0, 0);
		m_d3dContext->PSSetShader(0, 0, 0);
		m_d3dContext->VSSetConstantBuffers(
			0,
			1,
			0
			);*/
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