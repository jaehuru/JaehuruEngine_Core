#include "HighLevelInterface/IApplication.h"
#include "Renderer/RRenderer.h"
#include "Helpers/Input.h"
#include "Helpers/Time.h"
#include "Scene/JSceneManager.h"
#include "Resource/RResources.h"
#include "Collision/JCollisionManager.h"
#include "UI/UUIManager.h"
#include "FMOD/JFmod.h"
#include "Graphics/RRenderTarget.h"
#include "Resource/RTexture.h"


IApplication::IApplication() : 
	bLoaded(false),
	bRunning(false),
	bMinimized(false),
	mHwnd(nullptr),
	mWindowWidth(0),
	mWindowHeight(0),
	mWidth(0),
	mHeight(0),
	mX(0),
	mY(0)
{

}

IApplication::~IApplication()
{
		
}

void IApplication::Initialize(HWND hwnd, int width, int height)
{
	mHwnd = hwnd;

	AdjustWindowRect(hwnd, width, height);
	InitializeEtc();

	mGraphicDevice = make_unique<RGraphicDevice_DX11>();
	mGraphicDevice->Initialize();
	renderer::Initialize();

	JFmod::Initialize();
	JCollisionManager::Initialize();
	UUIManager::Initialize();
	JSceneManager::Initialize();

	bRunning = true;
}

void IApplication::InitializeWindow(HWND hwnd)
{
	SetWindowPos(hwnd, nullptr, mX, mY, mWindowWidth, mWindowHeight, 0);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

void IApplication::AdjustWindowRect(HWND hwnd, int width, int height)
{
	RECT rect = { 0, 0, (LONG)width, (LONG)height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	RECT winRect;
	::GetWindowRect(mHwnd, &winRect);

	//window position
	mX = winRect.left;
	mY = winRect.top;

	// window size
	mWindowWidth = rect.right - rect.left;
	mWindowHeight = rect.bottom - rect.top;

	//client size
	mWidth = width;
	mHeight = height;

	InitializeWindow(hwnd);
}

void IApplication::ReszieGraphicDevice()
{
	if (mGraphicDevice == nullptr)
		return;

	RECT winRect;
	::GetClientRect(mHwnd, &winRect);
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(winRect.right - winRect.left);
	viewport.Height = static_cast<float>(winRect.bottom - winRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	mWidth = (UINT)viewport.Width;
	mHeight = (UINT)viewport.Height;

	mGraphicDevice->Resize(viewport);
	renderer::FrameBuffer->Resize(mWidth, mHeight);
}

void IApplication::InitializeEtc()
{
	Input::Initialize();
	Time::Initialize();
}

void IApplication::Run()
{
	if (bLoaded == false)
		bLoaded = true;

	Update();
	LateUpdate();

	if (bMinimized == false)
	{
		Render();
	}

	Destroy();
}

void IApplication::Close()
{
	bRunning = false;
}

void IApplication::Update()
{
	Input::Update();
	Time::Update();
	JCollisionManager::Update();
	UUIManager::Update();
	JSceneManager::Update();
}

void IApplication::LateUpdate()
{
	JCollisionManager::LateUpdate();
	UUIManager::LateUpdate();
	JSceneManager::LateUpdate();
}

void IApplication::Render()
{
	GetDevice()->ClearRenderTargetView();
	GetDevice()->ClearDepthStencilView();
	GetDevice()->BindViewPort();
	GetDevice()->BindDefaultRenderTarget();

	Time::Render();
	JCollisionManager::Render();
	UUIManager::Render();
	JSceneManager::Render();

	//copy back buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> src = GetDevice()->GetFrameBuffer();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> dst = renderer::FrameBuffer->GetAttachmentTexture(0)->GetTexture();

	GetDevice()->CopyResource(dst.Get(), src.Get());
}

void IApplication::Present()
{
	GetDevice()->Present();
}

void IApplication::Destroy()
{
	JSceneManager::Destroy();
}

void IApplication::Release()
{
	JSceneManager::Release();
	UUIManager::Release();
	RResources::Release();
	renderer::Release();
}
