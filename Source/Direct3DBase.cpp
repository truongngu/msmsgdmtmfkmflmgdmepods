#include "DebugDefine.h"
#if defined WindowPhone
#include "Direct3DBase.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

//define for WindowPhone
#if !defined WindowStore
// Constructor.
Direct3DBase::Direct3DBase()
{
}

// Initialize the Direct3D resources required to run.
void Direct3DBase::Initialize(CoreWindow^ window)
{
	m_window = window;

	CreateDeviceResources();
#if defined WindowPhoneWithoutXAML
	CreateWindowSizeDependentResources();
#endif
}

// Recreate all device resources and set them back to the current state.
void Direct3DBase::HandleDeviceLost()
{
	// Reset these member variables to ensure that UpdateForWindowSizeChange recreates all resources.
	m_windowBounds.Width = 0;
	m_windowBounds.Height = 0;
	m_swapChain = nullptr;

	CreateDeviceResources();
	UpdateForWindowSizeChange();
}

// These are the resources that depend on the device.
void Direct3DBase::CreateDeviceResources()
{
	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers with this flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.3 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_9_3
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	DX::ThrowIfFailed(
		D3D11CreateDevice(
		nullptr, // Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags, // Set set debug and Direct2D compatibility flags.
		featureLevels, // List of feature levels this app can support.
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION.
		&device, // Returns the Direct3D device created.
		&m_featureLevel, // Returns feature level of device created.
		&context // Returns the device immediate context.
		)
		);

	// Get the Direct3D 11.1 API device and context interfaces.
	DX::ThrowIfFailed(
		device.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		context.As(&m_d3dContext)
		);
}

// Allocate all memory resources that depend on the window size.
void Direct3DBase::CreateWindowSizeDependentResources()
{
#if defined	 WindowPhoneWithoutXAML
	m_windowBounds = m_window->Bounds;
	float wid = 50;
	float hei = 50;
	// Calculate the necessary swap chain and render target size in pixels.
	m_renderTargetSize.Width = ConvertDipsToPixels(m_windowBounds.Width);
	m_renderTargetSize.Height = ConvertDipsToPixels(m_windowBounds.Height);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
	swapChainDesc.Width = static_cast<UINT>(m_renderTargetSize.Width); // Match the size of the window.
	swapChainDesc.Height = static_cast<UINT>(m_renderTargetSize.Height);
	
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1; // On phone, only single buffering is supported.
	swapChainDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH; // On phone, only stretch and aspect-ratio stretch scaling are allowed.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // On phone, no swap effects are supported.
	swapChainDesc.Flags = 0;
#endif	
	ComPtr<IDXGIDevice1> dxgiDevice;
	DX::ThrowIfFailed(
		m_d3dDevice.As(&dxgiDevice)
		);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	DX::ThrowIfFailed(
		dxgiDevice->GetAdapter(&dxgiAdapter)
		);

	ComPtr<IDXGIFactory2> dxgiFactory;
	DX::ThrowIfFailed(
		dxgiAdapter->GetParent(
		__uuidof(IDXGIFactory2),
		&dxgiFactory
		)
		);
#ifdef WindowPhoneWithoutXAML
	Windows::UI::Core::CoreWindow^ window = m_window.Get();
	DX::ThrowIfFailed(
		dxgiFactory->CreateSwapChainForCoreWindow(
		m_d3dDevice.Get(),
		reinterpret_cast<IUnknown*>(window),
		&swapChainDesc,
		nullptr, // Allow on all displays.
		&m_swapChain
		)
		);
#endif		
	// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
	// ensures that the application will only render after each VSync, minimizing power consumption.
	DX::ThrowIfFailed(
		dxgiDevice->SetMaximumFrameLatency(1)
		);
#ifdef WindowPhoneWithoutXAML
	// Create a render target view of the swap chain back buffer.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		&backBuffer
		)
		);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&m_renderTargetView
		)
		);
#endif
#ifdef WindowPhoneWithXAML
	// Create a descriptor for the render target buffer.
	CD3D11_TEXTURE2D_DESC renderTargetDesc(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);
	renderTargetDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;

	// Allocate a 2-D surface as the render target buffer.
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
		&renderTargetDesc,
		nullptr,
		&m_renderTarget
		)
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
		m_renderTarget.Get(),
		nullptr,
		&m_renderTargetView
		)
		);
#endif
	// Create a depth stencil view.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&depthStencil
		)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilView(
		depthStencil.Get(),
		&depthStencilViewDesc,
		&m_depthStencilView
		)
		);


	//create a Rasterizer view
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRasterizerState(
		&rasterDesc,
		&m_RasterizerState
		)
		);
	m_d3dContext->RSSetState(m_RasterizerState.Get());
	// Set the rendering viewport to target the entire window.
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_renderTargetSize.Width,
		m_renderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &viewport);
}

// This method is called in the event handler for the SizeChanged event.
void Direct3DBase::UpdateForWindowSizeChange()
{
	if (m_window->Bounds.Width != m_windowBounds.Width ||
		m_window->Bounds.Height != m_windowBounds.Height)
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_renderTargetView = nullptr;
		m_depthStencilView = nullptr;
		m_d3dContext->Flush();
		CreateWindowSizeDependentResources();
	}
}

void Direct3DBase::UpdateForRenderResolutionChange(float width, float height)
{
	if (m_d3dContext.Get()){
		m_renderTargetSize.Width = width;
		m_renderTargetSize.Height = height;

		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_renderTarget = nullptr;
		m_renderTargetView = nullptr;
		m_depthStencilView = nullptr;
		m_d3dContext->Flush();
		CreateWindowSizeDependentResources();
	}
}

void Direct3DBase::UpdateForWindowSizeChange(float width, float height)
{
	m_windowBounds.Width = width;
	m_windowBounds.Height = height;
}

void Direct3DBase::ReleaseResourcesForSuspending()
{
	// Phone applications operate in a memory-constrained environment, so when entering
	// the background it is a good idea to free memory-intensive objects that will be
	// easy to restore upon reactivation. The swapchain and backbuffer are good candidates
	// here, as they consume a large amount of memory and can be reinitialized quickly.
	m_swapChain = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilView = nullptr;
}

// Method to deliver the final image to the display.
void Direct3DBase::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// Discard the contents of the render target.
	// This is a valid operation only when the existing contents will be entirely
	// overwritten. If dirty or scroll rects are used, this call should be removed.
	m_d3dContext->DiscardView(m_renderTargetView.Get());

	// Discard the contents of the depth stencil.
	m_d3dContext->DiscardView(m_depthStencilView.Get());

	// If the device was removed either by a disconnect or a driver upgrade, we 
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED)
	{
		HandleDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Method to convert a length in device-independent pixels (DIPs) to a length in physical pixels.
float Direct3DBase::ConvertDipsToPixels(float dips)
{
	static const float dipsPerInch = 96.0f;
	return floor(dips * DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
}
#endif

//define for WindowStore
#if defined WindowStore
#include <windows.ui.xaml.media.dxinterop.h>
using namespace Windows::UI::Xaml::Controls;

Direct3DBase::Direct3DBase() :
m_dpi(-1.0f), m_logicalSize(),
m_compositionScaleX(1.0f),
m_compositionScaleY(1.0f),
m_orientation(Windows::Graphics::Display::DisplayOrientations::Portrait)
{
}

// Initialize the DirectX resources required to run.
void Direct3DBase::Initialize(CoreWindow^ window, SwapChainPanel^ panel, float dpi)
{
	m_window = window;
	m_panel = panel;

	CreateDeviceResources();
	SetDpi(dpi);
}

// Recreate all device resources and set them back to the current state.
void Direct3DBase::HandleDeviceLost()
{
	// Reset these member variables to ensure that SetDpi recreates all resources.
	float dpi = m_dpi;
	m_dpi = -1.0f;
	m_windowBounds.Width = 0;
	m_windowBounds.Height = 0;
	m_swapChain = nullptr;

	CreateDeviceResources();
	SetDpi(dpi);
}

// These are the resources that depend on the device.
void Direct3DBase::CreateDeviceResources()
{
	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	ComPtr<IDXGIDevice> dxgiDevice;

#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers with this flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> d3dDevice;
	ComPtr<ID3D11DeviceContext> d3dContext;
	DX::ThrowIfFailed(
		D3D11CreateDevice(
		nullptr, // Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags, // Set debug and Direct2D compatibility flags.
		featureLevels, // List of feature levels this app can support.
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		&d3dDevice, // Returns the Direct3D device created.
		&m_d3dFeatureLevel, // Returns feature level of device created.
		&d3dContext // Returns the device immediate context.
		)
		);

	// Get the Direct3D 11.1 API device and context interfaces.
	DX::ThrowIfFailed(
		d3dDevice.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		d3dContext.As(&m_d3dContext)
		);

	// Get the underlying DXGI device of the Direct3D device.
	DX::ThrowIfFailed(
		m_d3dDevice.As(&dxgiDevice)
		);
}

// This method is called in the event handler for the LogicalDpiChanged event.
void Direct3DBase::SetDpi(float dpi)
{
	if (dpi != m_dpi)
	{
		// Save the updated DPI value.
		m_dpi = dpi;

		// Update Direct2D's stored DPI.

		// Often a DPI change implies a window size change. In some cases Windows will issue
		// both a size changed event and a DPI changed event. In this case, the resulting bounds 
		// will not change, and the window resize code will only be executed once.
		UpdateForWindowSizeChange();
	}
}

void Direct3DBase::SetLogicalSize(Windows::Foundation::Size logicalSize){
	if (m_logicalSize != logicalSize)
	{
		m_logicalSize = logicalSize;
		CreateWindowSizeDependentResources();
	}
}

void Direct3DBase::SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations currentOrientation){
	if (m_orientation != currentOrientation)
	{
		m_orientation = currentOrientation;
		CreateWindowSizeDependentResources();
	}
}

void Direct3DBase::SetCompositionScale(float compositionScaleX, float compositionScaleY){

}

// This method is called in the event handler for the SizeChanged event.
void Direct3DBase::UpdateForWindowSizeChange()
{
	// Only handle window size changed if there is no pending DPI change.
	if (m_dpi != DisplayProperties::LogicalDpi)
	{
		return;
	}

	if (m_window->Bounds.Width != m_windowBounds.Width ||
		m_window->Bounds.Height != m_windowBounds.Height ||
		m_orientation != DisplayProperties::CurrentOrientation)
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_renderTargetView = nullptr;
		m_depthStencilView = nullptr;
		m_d3dContext->Flush();
		CreateWindowSizeDependentResources();
	}
}

// Allocate all memory resources that change on a window SizeChanged event.
void Direct3DBase::CreateWindowSizeDependentResources()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	m_renderTargetView = nullptr;
	m_depthStencilView = nullptr;
	m_d3dContext->Flush();
	// Store the window bounds so the next time we get a SizeChanged event we can
	// avoid rebuilding everything if the size is identical.
	m_windowBounds = m_window->Bounds;

	// Calculate the necessary swap chain and render target size in pixels.
	//float windowWidth = ConvertDipsToPixels(m_windowBounds.Width);
	//float windowHeight = ConvertDipsToPixels(m_windowBounds.Height);
	float windowWidth = m_logicalSize.Width * m_compositionScaleX;
	float windowHeight = m_logicalSize.Height * m_compositionScaleY;

	windowHeight = max(windowHeight, 1);
	windowWidth = max(windowWidth, 1);

	// The width and height of the swap chain must be based on the window's
	// landscape-oriented width and height. If the window is in a portrait
	// orientation, the dimensions must be reversed.
	//m_orientation = DisplayProperties::CurrentOrientation;
	bool swapDimensions =
		m_orientation == DisplayOrientations::Portrait ||
		m_orientation == DisplayOrientations::PortraitFlipped;
	m_renderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
	m_renderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;

	if (m_swapChain != nullptr)
	{
		// If the swap chain already exists, resize it.
		HRESULT hr = m_swapChain->ResizeBuffers(
			2, // Double-buffered swap chain.
			static_cast<UINT>(m_renderTargetSize.Width),
			static_cast<UINT>(m_renderTargetSize.Height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			HandleDeviceLost();
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// Otherwise, create a new one using the same adapter as the existing Direct3D device.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = static_cast<UINT>(m_renderTargetSize.Width); // Match the size of the window.
		swapChainDesc.Height = static_cast<UINT>(m_renderTargetSize.Height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
		swapChainDesc.Flags = 0;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(
			m_d3dDevice.As(&dxgiDevice)
			);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(
			dxgiDevice->GetAdapter(&dxgiAdapter)
			);

		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
			);

		//Windows::UI::Core::CoreWindow^ window = m_window.Get();
		//DX::ThrowIfFailed(
		//	dxgiFactory->CreateSwapChainForCoreWindow(
		//	m_d3dDevice.Get(),
		//	reinterpret_cast<IUnknown*>(window),
		//	&swapChainDesc,
		//	nullptr, // Allow on all displays.
		//	&m_swapChain
		//	)
		//	);

		DX::ThrowIfFailed(
			dxgiFactory->CreateSwapChainForComposition(
			m_d3dDevice.Get(),
			&swapChainDesc,
			nullptr,
			&m_swapChain
			)
			);

		// Associate the new swap chain with the SwapChainBackgroundPanel element.
		ComPtr<ISwapChainPanelNative> panelNative;
		DX::ThrowIfFailed(
			reinterpret_cast<IUnknown*>(m_panel)->QueryInterface(IID_PPV_ARGS(&panelNative))
			);

		DX::ThrowIfFailed(
			panelNative->SetSwapChain(m_swapChain.Get())
			);

		// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
		// ensures that the application will only render after each VSync, minimizing power consumption.
		DX::ThrowIfFailed(
			dxgiDevice->SetMaximumFrameLatency(1)
			);
	}

	// Set the proper orientation for the swap chain, and generate 2D and
	// 3D matrix transformations for rendering to the rotated swap chain.
	// Note the rotation angle for the 2D and 3D transforms are different.
	// This is due to the difference in coordinate spaces.  Additionally,
	// the 3D matrix is specified explicitly to avoid rounding errors.
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
	switch (m_orientation)
	{
	case DisplayOrientations::Landscape:
		rotation = DXGI_MODE_ROTATION_IDENTITY;
		
		m_orientationTransform3D = XMFLOAT4X4( // 0-degree Z-rotation
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		break;

	case DisplayOrientations::Portrait:
		rotation = DXGI_MODE_ROTATION_ROTATE270;
		
		m_orientationTransform3D = XMFLOAT4X4( // 90-degree Z-rotation
			0.0f, 1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		break;

	case DisplayOrientations::LandscapeFlipped:
		rotation = DXGI_MODE_ROTATION_ROTATE180;
		
		m_orientationTransform3D = XMFLOAT4X4( // 180-degree Z-rotation
			-1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		break;

	case DisplayOrientations::PortraitFlipped:
		rotation = DXGI_MODE_ROTATION_ROTATE90;
		
		m_orientationTransform3D = XMFLOAT4X4( // 270-degree Z-rotation
			0.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		break;

	default:
		throw ref new Platform::FailureException();
	}

	DX::ThrowIfFailed(
		m_swapChain->SetRotation(rotation)
		);

	// Create a Direct3D render target view of the swap chain back buffer.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&m_renderTargetView
		)
		);

	// Create a depth stencil view for use with 3D rendering if needed.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&depthStencil
		)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilView(
		depthStencil.Get(),
		&depthStencilViewDesc,
		&m_depthStencilView
		)
		);

	// Set the 3D rendering viewport to target the entire window.
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_renderTargetSize.Width,
		m_renderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &viewport);
}

// Method to deliver the final image to the display.
void Direct3DBase::Present()
{
	// The application may optionally specify "dirty" or "scroll"
	// rects to improve efficiency in certain scenarios.
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	parameters.DirtyRectsCount = 0;
	parameters.pDirtyRects = nullptr;
	parameters.pScrollRect = nullptr;
	parameters.pScrollOffset = nullptr;

	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present1(1, 0, &parameters);

	// Discard the contents of the render target.
	// This is a valid operation only when the existing contents will be entirely
	// overwritten. If dirty or scroll rects are used, this call should be removed.
	m_d3dContext->DiscardView(m_renderTargetView.Get());

	// Discard the contents of the depth stencil.
	m_d3dContext->DiscardView(m_depthStencilView.Get());

	// If the device was removed either by a disconnect or a driver upgrade, we 
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED)
	{
		HandleDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Method to convert a length in device-independent pixels (DIPs) to a length in physical pixels.
float Direct3DBase::ConvertDipsToPixels(float dips)
{
	static const float dipsPerInch = 96.0f;
	return floor(dips * m_dpi / dipsPerInch + 0.5f); // Round to nearest integer.
}


void Direct3DBase::ValidateDevice()
{
	ComPtr<IDXGIDevice1> dxgiDevice;
	ComPtr<IDXGIAdapter> deviceAdapter;
	DXGI_ADAPTER_DESC deviceDesc;
	DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));
	DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));
	DX::ThrowIfFailed(deviceAdapter->GetDesc(&deviceDesc));

	ComPtr<IDXGIFactory2> dxgiFactory;
	ComPtr<IDXGIAdapter1> currentAdapter;
	DXGI_ADAPTER_DESC currentDesc;
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
	DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, &currentAdapter));
	DX::ThrowIfFailed(currentAdapter->GetDesc(&currentDesc));

	if (deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(m_d3dDevice->GetDeviceRemovedReason()))
	{
		dxgiDevice = nullptr;
		deviceAdapter = nullptr;

		HandleDeviceLost();
	}
}

void Direct3DBase::UpdateForRenderResolutionChange(float width, float height)
{
	/*if (m_d3dContext.Get()){
		m_renderTargetSize.Width = width;
		m_renderTargetSize.Height = height;

		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_renderTarget = nullptr;
		m_renderTargetView = nullptr;
		m_depthStencilView = nullptr;
		m_d3dContext->Flush();
		CreateWindowSizeDependentResources();
	}*/
}

void Direct3DBase::ReleaseResourcesForSuspending()
{
	// Phone applications operate in a memory-constrained environment, so when entering
	// the background it is a good idea to free memory-intensive objects that will be
	// easy to restore upon reactivation. The swapchain and backbuffer are good candidates
	// here, as they consume a large amount of memory and can be reinitialized quickly.
	/*m_swapChain = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilView = nullptr;*/
}

void Direct3DBase::UpdateForWindowSizeChange(float width, float height)
{
	m_windowBounds.Width = width;
	m_windowBounds.Height = height;
}
#endif
#endif
