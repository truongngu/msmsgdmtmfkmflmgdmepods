#include "RectangleMesh.h"
#include "Global.h"

GLuint RectangleMesh::RectangleMeh_vbo=-1;
GLuint RectangleMesh::RectangleMesh_ido=-1;
int RectangleMesh::ReferenceCount=0;

#ifdef WindowPhone
#include "DirectXHelper.h"
#endif

RectangleMesh::RectangleMesh(float width,float height,float uvOffsetX,float uvOffsetY)
{
	Vertex verticesData[6];
	mNumVertices=6;
	float x=0,y=0,z=0;
	verticesData[0].position.x = x - width / 2;  verticesData[0].position.y = y + height / 2;  verticesData[0].position.z = z;
	verticesData[1].position.x = x - width / 2;  verticesData[1].position.y = y -height / 2;  verticesData[1].position.z = z;
	verticesData[2].position.x = x + width / 2;  verticesData[2].position.y = y -height / 2;  verticesData[2].position.z = z;

	verticesData[3].position.x = x - width / 2;  verticesData[3].position.y = y + height / 2;  verticesData[3].position.z = z;
	verticesData[4].position.x = x + width / 2;  verticesData[4].position.y = y + height / 2;  verticesData[4].position.z = z;
	verticesData[5].position.x = x + width / 2;  verticesData[5].position.y = y - height / 2;  verticesData[5].position.z = z;

	/*verticesData[0].position.x = 0.0f;  verticesData[0].position.y = height;  verticesData[0].position.z = z;
	verticesData[1].position.x = 0.0f;  verticesData[1].position.y = 0.0f;  verticesData[1].position.z = z;
	verticesData[2].position.x = width;  verticesData[2].position.y = 0.0f;  verticesData[2].position.z = z;

	verticesData[3].position.x = 0.0f;  verticesData[3].position.y = height;  verticesData[3].position.z = z;
	verticesData[4].position.x = width;  verticesData[4].position.y =0.0f;  verticesData[4].position.z = z;
	verticesData[5].position.x = width;  verticesData[5].position.y = height;  verticesData[5].position.z = z;*/



	float dx = float(1) / uvOffsetX;
	float dy = float(1) / uvOffsetY;
	//int i = 0; int j = 0;
	//verticesData[0].texcoord.x = 0;			verticesData[0].texcoord.y = 1;
	//verticesData[1].texcoord.x = 0;			verticesData[1].texcoord.y = 1 - dy;
	//verticesData[2].texcoord.x = dx;		verticesData[2].texcoord.y = 1 - dy;

	//verticesData[3].texcoord.x = 0;			verticesData[3].texcoord.y = 1;
	//verticesData[4].texcoord.x = dx;		verticesData[4].texcoord.y = 1-dy;
	//verticesData[5].texcoord.x = dx;		verticesData[5].texcoord.y = 1;
	int i = 0; int j = 0;
	verticesData[0].texcoord.x = i * dx;			verticesData[0].texcoord.y = 1 - j * dy;
	verticesData[1].texcoord.x = i * dx;			verticesData[1].texcoord.y = 1 - j * dy - dy;
	verticesData[2].texcoord.x = i * dx + dx;		verticesData[2].texcoord.y = 1 - j * dy - dy;

	verticesData[3].texcoord.x = i * dx;			verticesData[3].texcoord.y = 1 - j * dy;
	verticesData[4].texcoord.x = i * dx + dx;		verticesData[4].texcoord.y = 1 - j * dy;
	verticesData[5].texcoord.x = i * dx + dx;		verticesData[5].texcoord.y = 1 - j * dy - dy;
	/*verticesData[0].texcoord.x = 0.0f;				verticesData[0].texcoord.y = dy;
	verticesData[1].texcoord.x = 0.0f;			    verticesData[1].texcoord.y = 0.0f;
	verticesData[2].texcoord.x = 0.0f;		        verticesData[2].texcoord.y =dx;

	verticesData[3].texcoord.x = 0.0f;			verticesData[3].texcoord.y =dy;
	verticesData[4].texcoord.x = 0.0f;		verticesData[4].texcoord.y = dx;
	verticesData[5].texcoord.x =dx;		verticesData[5].texcoord.y = dy;*/


#if defined Win32 || Android
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

#ifdef WindowPhone
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = verticesData;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(verticesData), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		Global::m_d3ddevice->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
		)
		);

	unsigned short indices[] =
	{
		3, 4, 5,
		0, 2, 1
	};

	m_indexCount = ARRAYSIZE(indices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(indices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		Global::m_d3ddevice->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
		)
		);
#endif

	//vbo=RectangleMesh::RectangleMeh_vbo;
	mRenderType=MeshRenderType::RenderByArray;
	RectangleMesh::ReferenceCount+=1;
}

RectangleMesh::~RectangleMesh()
{
	
}