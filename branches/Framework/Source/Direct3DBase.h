﻿#pragma once
#include "DebugDefine.h"
#if defined WindowPhone
#include "DirectXHelper.h"
// Helper class that initializes DirectX APIs for 3D rendering.
class Direct3DBase abstract
{
public:
	Direct3DBase();
#if !defined WindowStore
	virtual void Initialize(Windows::UI::Core::CoreWindow^ window);
#endif
#if defined WindowStore
	virtual void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainPanel^ panel, float dpi);
	virtual void SetLogicalSize(Windows::Foundation::Size logicalSize);
	virtual void SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations currentOrientation);
	virtual void SetDpi(float dpi);
	virtual void SetCompositionScale(float compositionScaleX, float compositionScaleY);
	virtual void ValidateDevice();
#endif
	virtual void HandleDeviceLost();
	virtual void CreateDeviceResources();
	virtual void CreateWindowSizeDependentResources();
	virtual void UpdateForWindowSizeChange();	
	virtual void UpdateForRenderResolutionChange(float width, float height);
	virtual void UpdateForWindowSizeChange(float width, float height);
	virtual void ReleaseResourcesForSuspending();
	virtual void Render(float deltaTime) = 0;
	virtual void Present();
	virtual float ConvertDipsToPixels(float dips);
	Microsoft::WRL::ComPtr<ID3D11Device1> getDevice(){ return m_d3dDevice; }
	virtual ID3D11Texture2D* GetTexture()
	{
		return m_renderTarget.Get();
	}
protected:
	// Direct3D Objects.
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTarget;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;

	// Cached renderer properties.
	D3D_FEATURE_LEVEL m_featureLevel;
	Windows::Foundation::Size m_renderTargetSize;
	Windows::Foundation::Rect m_windowBounds;
	Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
#if defined WindowStore
	Windows::UI::Xaml::Controls::SwapChainPanel^ m_panel;
	float m_dpi;
	Windows::Foundation::Size						m_logicalSize;
	float											m_compositionScaleX;
	float											m_compositionScaleY;
	Windows::Graphics::Display::DisplayOrientations m_orientation;
	DirectX::XMFLOAT4X4 m_orientationTransform3D;
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;
#endif
};



struct VertexPositionColor
{
	VertexPositionColor(){
		pos = DirectX::XMFLOAT3();
		color = DirectX::XMFLOAT3();
	}
	VertexPositionColor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dcolor){
		pos = position;
		color = dcolor;
	}

	VertexPositionColor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 coord){
		pos = position;
		texCoord = coord;
	}

	VertexPositionColor(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), texCoord(u, v){}

	VertexPositionColor(const VertexPositionColor &vpc){
		pos = vpc.pos;
		color = vpc.color;
	}

	void operator = (VertexPositionColor vpc){
		pos = vpc.pos;
		color = vpc.color;
	}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 texCoord;
};
#endif