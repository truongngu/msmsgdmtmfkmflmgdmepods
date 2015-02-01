#pragma once
#include "DebugDefine.h"
#include "Shader.h"
#if defined Win32 || defined Android
#include "ft2build.h"
#include FT_FREETYPE_H
#endif

class TextRenderer
{
private:
	TextRenderer();
	static TextRenderer* instance;
	bool canDo;
#if defined Win32 || defined Android
	FT_Face face;
	FT_Library ftLib;
#endif
	Shaders* textShader;

	GLuint tex;
	GLuint vbo;
public:
	static TextRenderer* GetInstance();
	bool IsAvailable()
	{
		return canDo;
	}
	~TextRenderer();
	void InitFont(char* fontPath, float fontSize);
	void RenderText(const char *text, float x, float y, float sx, float sy);
	void RenderText(const char *text, float x, float y, Vector4 color);
	void RenderText(const char *text, Vector3 pos, Vector3 scale, Vector3 rot, Vector4 color);
};

