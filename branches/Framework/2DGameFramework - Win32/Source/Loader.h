#pragma once
#include "DebugDefine.h"
#if defined WindowPhone
#include "DDS.h"

#pragma comment(lib,"dxguid.lib")

// A simple loader class that provides support for loading shaders, textures,
// and meshes from files on disk. Provides synchronous and asynchronous methods.
class BasicLoader
{
public:
	static int nShaderLoading;
	static bool IsLoading;
	BasicLoader(
		_In_ ID3D11Device1* d3dDevice);

	concurrency::task<void> LoadTextureAsync(
		_In_ Platform::String^ filename,
		_Out_opt_ ID3D11Texture2D** texture,
		_Out_opt_ ID3D11ShaderResourceView** textureView
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[],
		_In_ uint32 layoutDescNumElements,
		_Out_ ID3D11VertexShader** shader,
		_Out_opt_ ID3D11InputLayout** layout
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_Out_ ID3D11PixelShader** shader
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_Out_ ID3D11ComputeShader** shader
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_Out_ ID3D11GeometryShader** shader
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_In_reads_opt_(numEntries) const D3D11_SO_DECLARATION_ENTRY* streamOutDeclaration,
		_In_ uint32 numEntries,
		_In_reads_opt_(numStrides) const uint32* bufferStrides,
		_In_ uint32 numStrides,
		_In_ uint32 rasterizedStream,
		_Out_ ID3D11GeometryShader** shader
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_Out_ ID3D11HullShader** shader
		);

	concurrency::task<void> LoadShaderAsync(
		_In_ Platform::String^ filename,
		_Out_ ID3D11DomainShader** shader
		);

	concurrency::task<void> LoadMeshAsync(
		_In_ Platform::String^ filename,
		_Out_ ID3D11Buffer** vertexBuffer,
		_Out_ ID3D11Buffer** indexBuffer,
		_Out_opt_ uint32* vertexCount,
		_Out_opt_ uint32* indexCount
		);

private:
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;

	template <class DeviceChildType>
	inline void SetDebugName(
		_In_ DeviceChildType* object,
		_In_ Platform::String^ name
		);

	Platform::String^ GetExtension(
		_In_ Platform::String^ filename
		);

	void CreateTexture(
		_In_ bool decodeAsDDS,
		_In_reads_bytes_(dataSize) byte* data,
		_In_ uint32 dataSize,
		_Out_opt_ ID3D11Texture2D** texture,
		_Out_opt_ ID3D11ShaderResourceView** textureView,
		_In_opt_ Platform::String^ debugName
		);

	void CreateInputLayout(
		_In_reads_bytes_(bytecodeSize) byte* bytecode,
		_In_ uint32 bytecodeSize,
		_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC* layoutDesc,
		_In_ uint32 layoutDescNumElements,
		_Out_ ID3D11InputLayout** layout
		);

	void CreateMesh(
		_In_ byte* meshData,
		_Out_ ID3D11Buffer** vertexBuffer,
		_Out_ ID3D11Buffer** indexBuffer,
		_Out_opt_ uint32* vertexCount,
		_Out_opt_ uint32* indexCount,
		_In_opt_ Platform::String^ debugName
		);
};
#endif