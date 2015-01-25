#pragma once
#include "ft2build.h"
#include "Shader.h"
#include FT_FREETYPE_H

class TextRenderer
{
private:
	TextRenderer();
	static TextRenderer* instance;
	bool canDo;
	FT_Face face;
	FT_Library ftLib;
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

