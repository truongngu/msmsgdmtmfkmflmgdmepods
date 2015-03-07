#include "Shader.h"
#include "Global.h"
#include "Utils.h"
#include "DebugDefine.h"
#ifdef WindowPhone
#include "Loader.h"
#endif
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
#if defined Win32 || defined Android
GLuint LoadShader ( GLenum type, char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;

	// Load the shader source
	FILE * pf;
	if (fopen_s(&pf, filename, "rb" ) != 0)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **)&shaderSrc, NULL );
	delete [] shaderSrc;

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
#ifdef Win32
			esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
#endif

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

/// \brief Create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
GLuint CreateProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
#ifdef Win32
			esLogMessage ( "Error linking program:\n%s\n", infoLog );            
#endif
			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}

#endif

void Shaders::Init(char * fileVertexShader, char * fileFragmentShader, int referenceCount)
{
	mfileVertextShader=new char[MAX_LEN];
	mfileFragmentShader=new char[MAX_LEN];
	strcpy(mfileFragmentShader,fileFragmentShader);
	strcpy(mfileVertextShader,fileVertexShader);
	mReferenceCount=referenceCount;
}

int Shaders::Load()
{
	if (isLoaded)
		return -3;
#if defined Win32
	char pathVertex[MAX_LEN];
	char pathFragment[MAX_LEN];
	strcpy(pathVertex,Global::gameResourceDir.c_str());
	strcpy(pathFragment,Global::gameResourceDir.c_str());
	strcat(pathVertex,mfileVertextShader);
	strcat(pathFragment,mfileFragmentShader);

	vertexShader = LoadShader(GL_VERTEX_SHADER, pathVertex);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, pathFragment);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = CreateProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	//ATTRIBUTE
	a_positon = glGetAttribLocation(program, "a_posL");
	a_uv = glGetAttribLocation(program, "a_uv");
	a_normal = glGetAttribLocation(program, "a_norL");
	a_binormal = glGetAttribLocation(program, "a_binorL");
	a_tangent = glGetAttribLocation(program, "a_tengL");
	u_World = glGetUniformLocation(program, "m_World");
	u_MatrixMVP = glGetUniformLocation(program, "m_MatrixMVP");
	u_Camera = glGetUniformLocation(program, "m_Camera");
	u_texture = glGetUniformLocation(program, "u_texture");
	uniformTankBoost = glGetUniformLocation(program, "u_isBoosted");
	//UNIFORM
	uniformA0=glGetUniformLocation(program,"a0");
	uniformA1=glGetUniformLocation(program,"a1");
	uniformA2=glGetUniformLocation(program,"a2");
	uniformColorText = glGetUniformLocation(program, "color");

	uniformLightDirect=glGetUniformLocation(program,"u_lightMainDirect");
	uniformLightSpecular=glGetUniformLocation(program,"u_lightSpecular");
	uniformLightAmbient=glGetUniformLocation(program,"u_lightAmbient");
	uniformLightColor=glGetUniformLocation(program,"u_lightColor");
	uniformLightPosition=glGetUniformLocation(program,"u_lightPosition");
	uniformCameraPosition=glGetUniformLocation(program,"u_camPosition");
	uniformObjectWorld=glGetUniformLocation(program,"u_W");
	u_MatrixMVP=glGetUniformLocation(program,"m_MatrixMVP");
	timeUniform=glGetUniformLocation(program,"u_time");
	cubeSampler=glGetUniformLocation(program,"u_samplerCubeMap");

	//Texture Sampler
	samples[0] = glGetUniformLocation(program, "u_blendmap");
	samples[1] = glGetUniformLocation(program, "u_texture_grass");
	samples[2] = glGetUniformLocation(program, "u_texture_dirt");
	samples[3] = glGetUniformLocation(program, "u_texture_rock");
	samples[4] = glGetUniformLocation(program, "u_texture_water");
	samples[5] = glGetUniformLocation(program, "u_texture");
	samples[6] = glGetUniformLocation(program, "u_normalMap");
	samples[7] = glGetUniformLocation(program, "u_diffuseMap");


	//
	u_Color = glGetUniformLocation(program, "u_Color");
	u_Tran = glGetUniformLocation(program, "u_Tran");
	u_IsProgress = glGetUniformLocation(program, "u_IsProgress");
	u_dx=glGetUniformLocation(program,"u_dx");
	u_dy=glGetUniformLocation(program,"u_dy");
#endif
#if defined WindowPhone
	BasicLoader loader = BasicLoader(Global::m_d3ddevice);
	//load pixel shader
	char pathVertex[MAX_LEN];
	char pathFragment[MAX_LEN];
	strcpy(pathVertex, Global::gameResourceDir.c_str());
	strcpy(pathFragment, Global::gameResourceDir.c_str());
	strcat(pathVertex, mfileVertextShader);
	strcat(pathFragment, mfileFragmentShader);
	try{
		loader.LoadShaderAsync(StringFromAscIIChars(mfileFragmentShader), &pixelShader);
	}
	catch (exception){
		return -1;
	}
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITAGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
	};
	try{
		loader.LoadShaderAsync(StringFromAscIIChars(mfileVertextShader), vertexDesc, ARRAYSIZE(vertexDesc), &vertexShader, &m_inputLayout);
	}
	catch (exception){
		return -2;
	}
#endif
	isLoaded=true;
	return 0;
}

Shaders::~Shaders()
{
	mReferenceCount--;
	if (mReferenceCount<=0)
	{
#if defined Win32
		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
#endif
#if defined WindowPhone
		vertexShader.ReleaseAndGetAddressOf();
		pixelShader.ReleaseAndGetAddressOf();
#endif
		Delete_Array(mfileVertextShader);
		Delete_Array(mfileFragmentShader);
	}
}

