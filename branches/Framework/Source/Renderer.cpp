#include "DebugDefine.h"
#if defined WindowPhone
#include "Renderer.h"
#include "Global.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

MyRenderer::MyRenderer()
{
	
}

void MyRenderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();
	Global::m_d3ddevice = m_d3dDevice.Get();
}

void MyRenderer::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();

	float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	float fovAngleY = 80.0f * XM_PI / 180.0f;
	if (aspectRatio < 1.0f)
	{
		fovAngleY /= aspectRatio;
	}

	/*Global::currentCamera->SetPosition(Vector3(0.0f, 1.0f, 11.1f));
	Global::currentCamera->projection.SetPerspective(fovAngleY, aspectRatio, 0.01f, 10000000.0f);
	Global::currentCamera->SetRotationAngle(Vector3(0, 0, PI / 2));*/
}

void MyRenderer::Update(float timeTotal, float timeDelta)
{
#if defined WindowPhoneWithoutXAML || defined WindowStore
	using namespace Windows::Graphics::Display;
	DirectX::XMFLOAT4X4 m_orientationTransform3D;
	DisplayOrientations m_orientation = DisplayProperties::CurrentOrientation;
	bool isLandscape = m_orientation == DisplayOrientations::Landscape;
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

	/*DX::ThrowIfFailed(
		m_swapChain->SetRotation(rotation)
		);*/
#endif
}

void MyRenderer::Render(float deltaTime)
{
	//const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	const float midnightBlue[] = { 1.000f , 1.000f , 1.000f , 1.000f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		midnightBlue
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);
	
	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

#if defined WindowPhoneWithoutXAML && !defined WindowStore
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_window->Bounds.Width,
		m_window->Bounds.Height
		);
	
	m_d3dContext->RSSetViewports(1, &viewport);
#endif
}
#endif