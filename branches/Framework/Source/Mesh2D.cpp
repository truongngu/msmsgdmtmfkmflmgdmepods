#include "Mesh2D.h"
#include "Global.h"

Mesh2D::Mesh2D()
{
#ifdef WindowPhone
	m_vertexBuffer.ReleaseAndGetAddressOf();
	m_indexBuffer.ReleaseAndGetAddressOf();
#endif
	vbo=-1;
	ido=-1;
	mNumIndex=0;
	mNumVertices=0;
	mRenderType=MeshRenderType::RenderByArray;
	mReferenceCount=0;
}

Mesh2D::~Mesh2D()
{
	if (mReferenceCount<=0)
	{
#ifdef Win32
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ido);
#endif
#ifdef WindowPhone
		m_vertexBuffer.ReleaseAndGetAddressOf();
		m_indexBuffer.ReleaseAndGetAddressOf();
#endif
	}
}

void Mesh2D::Load(string path2Mesh, MeshRenderType type)
{
	
}

void Mesh2D::Render(Shaders* shader)
{
#if defined Win32 || defined Android
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (shader->a_positon != -1)
	{
		glEnableVertexAttribArray(shader->a_positon);
		glVertexAttribPointer(shader->a_positon, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0);
	}
	//Vertex uv
	if (shader->a_uv != -1)
	{
		glEnableVertexAttribArray(shader->a_uv);
		glVertexAttribPointer(shader->a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));
	}
	//Vertex normal
	if (shader->a_normal != -1)
	{
		glEnableVertexAttribArray(shader->a_normal);
		glVertexAttribPointer(shader->a_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3)+sizeof(Vector2));
	}
	//Vertex binormal
	if (shader->a_binormal != -1)
	{
		glEnableVertexAttribArray(shader->a_normal);
		glVertexAttribPointer(shader->a_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3)* 2 + sizeof(Vector2));
	}
	//Vertex tangent
	if (shader->a_tangent != -1)
	{
		glEnableVertexAttribArray(shader->a_tangent);
		glVertexAttribPointer(shader->a_tangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3)* 3 + sizeof(Vector2));
	}
	if (mRenderType == MeshRenderType::RenderByArray)
	{
		glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
	}
	else if (mRenderType == MeshRenderType::RenderByIndex)
	{
		glBindBuffer(GL_TRIANGLES, ido);
		glDrawElements(mDrawMode, 0, GL_UNSIGNED_SHORT, 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
#if defined WindowPhone
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Global::m_d3ddevice->GetImmediateContext1(&m_d3dContext);

	UINT stride = sizeof(Vertex);
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

	m_d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dContext->DrawIndexed(
		m_indexCount,
		0,
		0
		);	
#endif
}






