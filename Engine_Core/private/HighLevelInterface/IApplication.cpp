#include "HighLevelInterface/IApplication.h"
#include "Renderer/RRenderer.h"
#include "Helpers/Input.h"
#include "Helpers/Time.h"
#include "Scene/JSceneManager.h"
#include "Resource/RResources.h"
#include "Collision/JCollisionManager.h"
#include "UI/UUIManager.h"
#include "FMOD/JFmod.h"


IApplication::IApplication() : 
	mbLoaded(false),
	mbRunning(false),
	mbMinimized(false),
	mHwnd(nullptr),
	mWidth(0),
	mHeight(0)
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

	mbRunning = true;
}

void IApplication::AdjustWindowRect(HWND hwnd, int width, int height)
{
	RECT rect = { 0, 0, (LONG)width, (LONG)height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::GetWindowRect(hwnd, &rect);

	int x = rect.left;
	int y = rect.top;

	mWidth = rect.right - rect.left;
	mHeight = rect.bottom - rect.top;

	SetWindowPos(hwnd, nullptr, x, y, mWidth, mHeight, 0);
	ShowWindow(hwnd, true);
}

void IApplication::ReszieGraphicDevice()
{
	if (mGraphicDevice == nullptr)
		return;

	RECT winRect;
	GetClientRect(mHwnd, &winRect);
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
}

void IApplication::InitializeEtc()
{
	Input::Initialize();
	Time::Initialize();
}

void IApplication::Run()
{
	if (mbLoaded == false)
		mbLoaded = true;

	Update();
	LateUpdate();

	if (mbMinimized == false)
	{
		Render();
	}

	Destroy();
}

void IApplication::Close()
{
	mbRunning = false;
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

	GetDevice()->Present();
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
