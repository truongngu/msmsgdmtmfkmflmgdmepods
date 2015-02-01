#include "DebugDefine.h"
#if defined WindowPhone
#include "Loader.h"
#include "Global.h"
#include <memory>

using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace std;
using namespace concurrency;

int BasicLoader::nShaderLoading = 0;
bool BasicLoader::IsLoading = false;
BasicLoader::BasicLoader(_In_ ID3D11Device1* d3dDevice) :
m_d3dDevice(d3dDevice)
{

}

template <class DeviceChildType>
inline void BasicLoader::SetDebugName(
	_In_ DeviceChildType* object,
	_In_ Platform::String^ name
	)
{
#if defined(_DEBUG)
	// Only assign debug names in debug builds.

	char nameString[1024];
	int nameStringLength = WideCharToMultiByte(
		CP_ACP,
		0,
		name->Data(),
		-1,
		nameString,
		1024,
		nullptr,
		nullptr
		);

	if (nameStringLength == 0)
	{
		char defaultNameString[] = "BasicLoaderObject";
		DX::ThrowIfFailed(
			object->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			sizeof(defaultNameString)-1,
			defaultNameString
			)
			);
	}
	else
	{
		DX::ThrowIfFailed(
			object->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			nameStringLength - 1,
			nameString
			)
			);
	}
#endif
}

Platform::String^ BasicLoader::GetExtension(
	_In_ Platform::String^ filename
	)
{
	int lastDotIndex = -1;
	for (int i = filename->Length() - 1; i >= 0 && lastDotIndex == -1; i--)
	{
		if (*(filename->Data() + i) == '.')
		{
			lastDotIndex = i;
		}
	}
	if (lastDotIndex != -1)
	{
		std::unique_ptr<wchar_t[]> extension(new wchar_t[filename->Length() - lastDotIndex]);
		for (unsigned int i = 0; i < filename->Length() - lastDotIndex; i++)
		{
			extension[i] = tolower(*(filename->Data() + lastDotIndex + 1 + i));
		}
		return ref new Platform::String(extension.get());
	}
	return "";
}

void BasicLoader::CreateTexture(
	_In_ bool decodeAsDDS,
	_In_reads_bytes_(dataSize) byte* data,
	_In_ uint32 dataSize,
	_Out_opt_ ID3D11Texture2D** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_In_opt_ Platform::String^ debugName
	)
{
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	ComPtr<ID3D11Texture2D> texture2D;

	if (decodeAsDDS)
	{
		ComPtr<ID3D11Resource> resource;

		if (textureView == nullptr)
		{
			CreateDDSTextureFromMemory(
				m_d3dDevice.Get(),
				data,
				dataSize,
				&resource,
				nullptr, 2048);
		}
		else
		{
			CreateDDSTextureFromMemory(
				m_d3dDevice.Get(),
				data,
				dataSize,
				&resource,
				&shaderResourceView,
				2048);
		}

		DX::ThrowIfFailed(
			resource.As(&texture2D)
			);
	}

	SetDebugName(texture2D.Get(), debugName);

	if (texture != nullptr)
	{
		*texture = texture2D.Detach();
	}
	if (textureView != nullptr)
	{
		*textureView = shaderResourceView.Detach();
	}
}

void BasicLoader::CreateInputLayout(
	_In_reads_bytes_(bytecodeSize) byte* bytecode,
	_In_ uint32 bytecodeSize,
	_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC* layoutDesc,
	_In_ uint32 layoutDescNumElements,
	_Out_ ID3D11InputLayout** layout
	)
{
	if (layoutDesc == nullptr)
	{
		// If no input layout is specified, use the BasicVertex layout.
		const D3D11_INPUT_ELEMENT_DESC basicVertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreateInputLayout(
			basicVertexLayoutDesc,
			ARRAYSIZE(basicVertexLayoutDesc),
			bytecode,
			bytecodeSize,
			layout
			)
			);
	}
	else
	{
		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreateInputLayout(
			layoutDesc,
			layoutDescNumElements,
			bytecode,
			bytecodeSize,
			layout
			)
			);
	}
}

void BasicLoader::CreateMesh(
	_In_ byte* meshData,
	_Out_ ID3D11Buffer** vertexBuffer,
	_Out_ ID3D11Buffer** indexBuffer,
	_Out_opt_ uint32* vertexCount,
	_Out_opt_ uint32* indexCount,
	_In_opt_ Platform::String^ debugName
	)
{
	// The first 4 bytes of the BasicMesh format define the number of vertices in the mesh.
	uint32 numVertices = *reinterpret_cast<uint32*>(meshData);

	// The following 4 bytes define the number of indices in the mesh.
	uint32 numIndices = *reinterpret_cast<uint32*>(meshData + sizeof(uint32));

	// The next segment of the BasicMesh format contains the vertices of the mesh.
	VertexPositionColor* vertices = reinterpret_cast<VertexPositionColor*>(meshData + sizeof(uint32)* 2);

	// The last segment of the BasicMesh format contains the indices of the mesh.
	uint16* indices = reinterpret_cast<uint16*>(meshData + sizeof(uint32)* 2 + sizeof(VertexPositionColor)* numVertices);

	// Create the vertex and index buffers with the mesh data.

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(numVertices * sizeof(VertexPositionColor), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		vertexBuffer
		)
		);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(numIndices * sizeof(uint16), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		indexBuffer
		)
		);

	SetDebugName(*vertexBuffer, Platform::String::Concat(debugName, "_VertexBuffer"));
	SetDebugName(*indexBuffer, Platform::String::Concat(debugName, "_IndexBuffer"));

	if (vertexCount != nullptr)
	{
		*vertexCount = numVertices;
	}
	if (indexCount != nullptr)
	{
		*indexCount = numIndices;
	}
}



task<void> BasicLoader::LoadTextureAsync(
	_In_ Platform::String^ filename,
	_Out_opt_ ID3D11Texture2D** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureView
	)
{
	IsLoading = true;
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ textureData)
	{
		CreateTexture(
			GetExtension(filename) == "tga",
			textureData->Data,
			textureData->Length,
			texture,
			textureView,
			filename
			);
	}).then([this](){
		IsLoading = false;
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[],
	_In_ uint32 layoutDescNumElements,
	_Out_ ID3D11VertexShader** shader,
	_Out_opt_ ID3D11InputLayout** layout
	)
{
	// This method assumes that the lifetime of input arguments may be shorter
	// than the duration of this task.  In order to ensure accurate results, a
	// copy of all arguments passed by pointer must be made.  The method then
	// ensures that the lifetime of the copied data exceeds that of the task.

	// Create copies of the layoutDesc array as well as the SemanticName strings,
	// both of which are pointers to data whose lifetimes may be shorter than that
	// of this method's task.
	shared_ptr<vector<D3D11_INPUT_ELEMENT_DESC>> layoutDescCopy;
	shared_ptr<vector<string>> layoutDescSemanticNamesCopy;
	if (layoutDesc != nullptr)
	{
		layoutDescCopy.reset(
			new vector<D3D11_INPUT_ELEMENT_DESC>(
			layoutDesc,
			layoutDesc + layoutDescNumElements
			)
			);

		layoutDescSemanticNamesCopy.reset(
			new vector<string>(layoutDescNumElements)
			);

		for (uint32 i = 0; i < layoutDescNumElements; i++)
		{
			layoutDescSemanticNamesCopy->at(i).assign(layoutDesc[i].SemanticName);
		}
	}

	//IsLoading = true;
	nShaderLoading++;
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		while (IsLoading) {}
		IsLoading = true;
		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreateVertexShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);

		if (layout != nullptr)
		{
			if (layoutDesc != nullptr)
			{
				// Reassign the SemanticName elements of the layoutDesc array copy to point
				// to the corresponding copied strings. Performing the assignment inside the
				// lambda body ensures that the lambda will take a reference to the shared_ptr
				// that holds the data.  This will guarantee that the data is still valid when
				// CreateInputLayout is called.
				for (uint32 i = 0; i < layoutDescNumElements; i++)
				{
					layoutDescCopy->at(i).SemanticName = layoutDescSemanticNamesCopy->at(i).c_str();
				}
			}

			CreateInputLayout(
				bytecode->Data,
				bytecode->Length,
				layoutDesc == nullptr ? nullptr : layoutDescCopy->data(),
				layoutDescNumElements,
				layout
				);

			SetDebugName(*layout, filename);
			//IsLoading = false;
		}
	}).then([this](){
		IsLoading = false;
		nShaderLoading--;
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_Out_ ID3D11PixelShader** shader
	)
{
	//IsLoading = true;
	nShaderLoading++;
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		while (IsLoading) {}
		IsLoading = true;
		DX::ThrowIfFailed(
			Global::m_d3ddevice->CreatePixelShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);
		//IsLoading = false;
	}).then([this](){
		IsLoading = false;
		nShaderLoading--;
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_Out_ ID3D11ComputeShader** shader
	)
{
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		DX::ThrowIfFailed(
			m_d3dDevice->CreateComputeShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_Out_ ID3D11GeometryShader** shader
	)
{
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		DX::ThrowIfFailed(
			m_d3dDevice->CreateGeometryShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_In_reads_opt_(numEntries) const D3D11_SO_DECLARATION_ENTRY* streamOutDeclaration,
	_In_ uint32 numEntries,
	_In_reads_opt_(numStrides) const uint32* bufferStrides,
	_In_ uint32 numStrides,
	_In_ uint32 rasterizedStream,
	_Out_ ID3D11GeometryShader** shader
	)
{
	// This method assumes that the lifetime of input arguments may be shorter
	// than the duration of this task.  In order to ensure accurate results, a
	// copy of all arguments passed by pointer must be made.  The method then
	// ensures that the lifetime of the copied data exceeds that of the task.

	// Create copies of the streamOutDeclaration array as well as the SemanticName
	// strings, both of which are pointers to data whose lifetimes may be shorter
	// than that of this method's task.
	shared_ptr<vector<D3D11_SO_DECLARATION_ENTRY>> streamOutDeclarationCopy;
	shared_ptr<vector<string>> streamOutDeclarationSemanticNamesCopy;
	if (streamOutDeclaration != nullptr)
	{
		streamOutDeclarationCopy.reset(
			new vector<D3D11_SO_DECLARATION_ENTRY>(
			streamOutDeclaration,
			streamOutDeclaration + numEntries
			)
			);

		streamOutDeclarationSemanticNamesCopy.reset(
			new vector<string>(numEntries)
			);

		for (uint32 i = 0; i < numEntries; i++)
		{
			streamOutDeclarationSemanticNamesCopy->at(i).assign(streamOutDeclaration[i].SemanticName);
		}
	}

	// Create a copy of the bufferStrides array, which is a pointer to data
	// whose lifetime may be shorter than that of this method's task.
	shared_ptr<vector<uint32>> bufferStridesCopy;
	if (bufferStrides != nullptr)
	{
		bufferStridesCopy.reset(
			new vector<uint32>(
			bufferStrides,
			bufferStrides + numStrides
			)
			);
	}

	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		if (streamOutDeclaration != nullptr)
		{
			// Reassign the SemanticName elements of the streamOutDeclaration array copy to
			// point to the corresponding copied strings. Performing the assignment inside the
			// lambda body ensures that the lambda will take a reference to the shared_ptr
			// that holds the data.  This will guarantee that the data is still valid when
			// CreateGeometryShaderWithStreamOutput is called.
			for (uint32 i = 0; i < numEntries; i++)
			{
				streamOutDeclarationCopy->at(i).SemanticName = streamOutDeclarationSemanticNamesCopy->at(i).c_str();
			}
		}

		DX::ThrowIfFailed(
			m_d3dDevice->CreateGeometryShaderWithStreamOutput(
			bytecode->Data,
			bytecode->Length,
			streamOutDeclaration == nullptr ? nullptr : streamOutDeclarationCopy->data(),
			numEntries,
			bufferStrides == nullptr ? nullptr : bufferStridesCopy->data(),
			numStrides,
			rasterizedStream,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_Out_ ID3D11HullShader** shader
	)
{
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		DX::ThrowIfFailed(
			m_d3dDevice->CreateHullShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);
	});
}

task<void> BasicLoader::LoadShaderAsync(
	_In_ Platform::String^ filename,
	_Out_ ID3D11DomainShader** shader
	)
{
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ bytecode)
	{
		DX::ThrowIfFailed(
			m_d3dDevice->CreateDomainShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			shader
			)
			);

		SetDebugName(*shader, filename);
	});
}

task<void> BasicLoader::LoadMeshAsync(
	_In_ Platform::String^ filename,
	_Out_ ID3D11Buffer** vertexBuffer,
	_Out_ ID3D11Buffer** indexBuffer,
	_Out_opt_ uint32* vertexCount,
	_Out_opt_ uint32* indexCount
	)
{
	return DX::ReadDataAsync(filename).then([=](const Platform::Array<byte>^ meshData)
	{
		CreateMesh(
			meshData->Data,
			vertexBuffer,
			indexBuffer,
			vertexCount,
			indexCount,
			filename
			);
	});
}

#endif