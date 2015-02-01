#include "DebugDefine.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "Global.h"
#include "Utils.h"
#if defined WindowPhone
#include "Global.h"
#include <mutex>
#include "DirectXHelper.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
#endif

TextRenderer* TextRenderer::instance = 0;
TextRenderer::TextRenderer()
{
#if defined Win32 || defined Android || defined WindowPhone
	//FT_Library ft;
	canDo = false;
	if (FT_Init_FreeType(&ftLib)) {
		Log("Could not init freetype library\n");
		return;
	}
	char path[255];
	strcpy(path, Global::gameResourceDir.c_str());
	strcat(path, "Resources//Fonts//times.ttf");

	if (FT_New_Face(ftLib, path, 0, &face)) {
		Log("Could not open font\n");
		return;
	}
	else
	{
		FT_Set_Pixel_Sizes(face, 0, 48);
		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
			Log("Could not load character 'X'\n");
			return;
		}
	}
	canDo = true;
	textShader = Global::TextShader;
#if defined Win32 || defined Android
	glGenTextures(1, &tex);
	glGenBuffers(1, &vbo);
#endif
#if defined WindowPhone
	//tao texture

	//tao buffer cho shader
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(Vector4), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		Global::m_d3ddevice->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		&m_constantBuffer
		)
		);

	GLfloat box[4][4] = {
		{ 1, -1, 0, 0 },
		{ 1, -1, 1, 0 },
		{ 1, -1, 0, 1 },
		{ 1, -1, 1, 1 },
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = box;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(box), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		Global::m_d3ddevice->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
		)
		);
	unsigned short indices[] =
	{
		1, 0, 2,
		1, 2, 3
	};

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(indices), D3D11_BIND_INDEX_BUFFER);
	int m_indexCount = ARRAYSIZE(indices);
	
	DX::ThrowIfFailed(
		Global::m_d3ddevice->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
		)
		);
#endif
#endif
}


TextRenderer::~TextRenderer()
{
	//Delete(textShader);
#if defined Win32 || defined Android
	glDeleteBuffers(1, &tex);
	glDeleteBuffers(1, &vbo);
	FT_Done_Face(face);
	FT_Done_FreeType(ftLib);
#endif
#if defined WindowPhone
	this->m_Texture.ReleaseAndGetAddressOf();
	this->SRV.ReleaseAndGetAddressOf();
	this->TexSamplerState.ReleaseAndGetAddressOf();
	m_vertexBuffer.ReleaseAndGetAddressOf();
	m_indexBuffer.ReleaseAndGetAddressOf();
	m_constantBuffer.ReleaseAndGetAddressOf();
#endif
}


void TextRenderer::InitFont(char* fontPath, float fontSize)
{
	if (!canDo)
		return;
	char path[255];
	strcpy(path, Global::gameResourceDir.c_str());
	strcat(path, fontPath);
	Log(fontPath);
	if (FT_New_Face(ftLib, path, 0, &face)) {
		Log("Could not open font\n");
		canDo = false;
	}
	else
	{
		FT_Set_Pixel_Sizes(face, 0, fontSize);
		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
			Log("Could not load character 'X'\n");
		}
		canDo = true;
	}
	if (!canDo)
	{
		if (FT_New_Face(ftLib, "Resources//Fonts//times.ttf", 0, &face)) {
			Log("Could not open font\n");
			return;
		}
		else
		{
			FT_Set_Pixel_Sizes(face, 0, 48);
			if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
				Log("Could not load character 'X'\n");
				return;
			}
		}
	}
	textShader = Global::TextShader;
	canDo = true;
}

TextRenderer* TextRenderer::GetInstance()
{
	if (!instance)
		instance = new TextRenderer();
	return instance;
}


void TextRenderer::RenderText(const char *text, float x, float y, float sx, float sy)
{
	if (!canDo)
		return;
#if defined Win32 || defined Android
	glUseProgram(Global::TextShader->program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnableVertexAttribArray(textShader->a_positon);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(textShader->a_positon, 4, GL_FLOAT, GL_FALSE, 0, 0);
#endif
	//Init diffuse color: i.e White
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
#if defined Win32 || defined Android
	if (textShader->uniformColorText != -1)
	{
		glUniform4fv(textShader->uniformColorText, 1, (GLfloat*)&color);
	}
#endif
	const char *p;
	FT_GlyphSlot glyph = face->glyph;
	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;
#if defined Win32 || defined Android
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			glyph->bitmap.buffer
			);
#endif


		float x2 = x + glyph->bitmap_left * sx;
		float y2 = -y - glyph->bitmap_top * sy;
		float w = glyph->bitmap.width * sx;
		float h = glyph->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};
#if defined Win32 || defined Android
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#endif
		x += (glyph->advance.x >> 6) * sx;
		y += (glyph->advance.y >> 6) * sy;
#if defined Win32 || defined Android
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}
#if defined Win32 || defined Android
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteBuffers(1, &vbo);
	glDisable(GL_BLEND);
#endif
	//delete g;
	//delete g;
}

void TextRenderer::RenderText(const char *text, float x, float y, Vector4 color)
{
	if (!canDo)
		return;
	float sx = 2.0f / Global::ScreenWidth;
	float sy = 2.0f / Global::ScreenHeight;
#if defined Win32 || defined Android
	glUseProgram(Global::TextShader->program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnableVertexAttribArray(textShader->a_positon);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(textShader->a_positon, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//Init diffuse color: i.e White
	//Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	if (textShader->uniformColorText != -1)
	{
		glUniform4fv(textShader->uniformColorText, 1, (GLfloat*)&color);
	}
#endif
#if defined WindowPhone
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);

	m_d3dContext->IASetInputLayout(Global::TextShader->m_inputLayout.Get());

	m_d3dContext->VSSetShader(
		Global::TextShader->vertexShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->PSSetShader(
		Global::TextShader->pixelShader.Get(),
		nullptr,
		0
		);
	//set mau vao cho texture cua Text
#endif
	const char *p;
	FT_GlyphSlot glyph = face->glyph;
	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;
#if defined WindowPhone
		if (!glyph || glyph->bitmap.width <= 0 || glyph->bitmap.rows <= 0)
			continue;
#endif
#if defined Win32 || defined Android
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			glyph->bitmap.buffer
			);
#endif
#if defined WindowPhone
		//get Matrix by view
		float sizeBitmap = (glyph->bitmap.width * glyph->bitmap.rows) / 4;
		std::mutex  m_mutex;
		std::lock_guard<std::mutex> lock(m_mutex);
		CD3D11_TEXTURE2D_DESC textureDesc = CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT::DXGI_FORMAT_A8_UNORM,
			static_cast<UINT>(glyph->bitmap.width),
			static_cast<UINT>(glyph->bitmap.rows),
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
		data.pSysMem = glyph->bitmap.buffer;
		if (glyph->bitmap.width > 0 && glyph->bitmap.rows > 0)
			data.SysMemPitch = 1 * glyph->bitmap.width;
		else{

		}
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

		//Delete_Array(imageData);
#endif

		float x2 = x + glyph->bitmap_left * sx;
		float y2 = -y - glyph->bitmap_top * sy;
		float w = glyph->bitmap.width * sx;
		float h = glyph->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};
#if defined Win32 || defined Android
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#endif
#if defined WindowPhone
		m_d3dContext->PSSetShaderResources(0, 1, SRV.GetAddressOf());
		m_d3dContext->PSSetSamplers(0, 1, TexSamplerState.GetAddressOf());

		m_d3dContext->UpdateSubresource(
			m_constantBuffer.Get(),
			0,
			NULL,
			&color,
			0,
			0
			);

		m_d3dContext->PSSetConstantBuffers(
			0,
			1,
			m_constantBuffer.GetAddressOf()
			);
		Vertex verticesData[6];

		Vector4 boxs[4] = {
			Vector4(x2, -y2, 0, 0),
			Vector4(x2 + w, -y2, 1, 0),
			Vector4(x2, -y2 - h, 0, 1 ),
			Vector4(x2 + w, -y2 - h, 1, 1),
		};

		m_d3dContext->UpdateSubresource(
			m_vertexBuffer.Get(),
			0,
			NULL,
			&boxs,
			0,
			0
			);

		/*m_d3dContext->UpdateSubresource(
			m_indexBuffer.Get(),
			0,
			NULL,
			&box,
			0,
			0
			);*/
		
		UINT stride = sizeof(boxs);
		UINT offset = 0;
		m_d3dContext->IASetVertexBuffers(
			0,
			1,
			m_vertexBuffer.GetAddressOf(),
			&stride,
			&offset
			);

		m_d3dContext->IASetIndexBuffer(
			m_indexBuffer.Get(),
			DXGI_FORMAT_R16_UINT,
			0
			);

		m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		m_d3dContext->DrawIndexed(6, 0, 0);
#endif
		x += (glyph->advance.x >> 6) * sx;
		y += (glyph->advance.y >> 6) * sy;
#if defined Win32 || defined Android
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}
#if defined Win32 || defined Android
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteBuffers(1, &vbo);
	glDisable(GL_BLEND);
#endif
#if defined WindowPhone
	//Microsoft::WRL::ComPtr<ID3D11Resource> resourse;
	//SRV.Get()->GetResource(resourse.GetAddressOf());
	//m_d3dContext->PSSetShaderResources(0, 1, 0);
	//m_d3dContext->DiscardResource(resourse.Get());
	//m_Texture.Get()->Release();
	//m_Texture.ReleaseAndGetAddressOf();
	//SRV.Get()->Release();
	//m_d3dContext->PSSetSamplers(0, 1, 0);
	//m_d3dContext->Unmap(resourse.Get(), 0);
#endif
	//delete g;
	//delete g;
}

void TextRenderer::RenderText(const char *text, Vector3 pos, Vector3 scale, Vector3 rot, Vector4 color)
{
#ifdef Win32
	if (!canDo)
		return;
	Vector3 camPos = Global::currentCamera->GetPosition();
	float sx = 2.0f / Global::ScreenWidth;
	float sy = 2.0f / Global::ScreenHeight;
	glUseProgram(Global::TextShaderInWorld->program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnableVertexAttribArray(Global::TextShaderInWorld->a_positon);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(Global::TextShaderInWorld->a_positon, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//Init diffuse color: i.e White
	//Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	if (Global::TextShaderInWorld->uniformColorText != -1)
	{
		glUniform4fv(Global::TextShaderInWorld->uniformColorText, 1, (GLfloat*)&color);
	}

	Matrix World;
	World.SetIdentity();
	//World.SetScale(10.0f);
	scale.x /= camPos.z;
	scale.y /= camPos.z;

	Matrix Scale = Matrix().SetIdentity().SetScale(scale);
	Matrix Translate = Translate.SetIdentity();
	Translate.SetTranslation(pos);
	Matrix RotationX, RotationY, RotationZ;
	RotationX.SetIdentity();
	RotationY.SetIdentity();
	RotationZ.SetIdentity();
	RotationX.SetRotationX(rot.x);
	RotationY.SetRotationY(rot.y);
	RotationZ.SetRotationZ(rot.z);
	Matrix Rotation = RotationZ*RotationX*RotationY;
	World = Scale*Rotation*Translate;

	Matrix MVP = World*Global::currentCamera->ViewMatrix() * Global::currentCamera->projection;
	if (Global::TextShaderInWorld->u_MatrixMVP != -1)	// Send MVP
		glUniformMatrix4fv(Global::TextShaderInWorld->u_MatrixMVP, 1, GL_FALSE, (float*)&MVP);

	const char *p;
	float x1 = ConvertCoordinate3D2D(Global::currentCamera, pos).x;
	float y1 = ConvertCoordinate3D2D(Global::currentCamera, pos).y;

	double x = 2.0 * x1 / Global::ScreenWidth - 1;
	double y = -2.0 * y1 / Global::ScreenHeight + 1;
	x = 0.0;
	y = 0.0;
	FT_GlyphSlot glyph = face->glyph;
	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			glyph->bitmap.buffer
			);



		float x2 = x + glyph->bitmap_left;
		float y2 = -y - glyph->bitmap_top;
		float w = glyph->bitmap.width;
		float h = glyph->bitmap.rows;

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (glyph->advance.x >> 6);
		y += (glyph->advance.y >> 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteBuffers(1, &vbo);
	glDisable(GL_BLEND);
	//delete g;
	//delete g;
#endif
}