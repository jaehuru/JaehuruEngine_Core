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
	mHwnd(nullptr),
	mHdc(nullptr),
	mWidth(0),
	mHeight(0),
	mBackHdc(NULL),
	mBackBitmap(NULL)
{

}

IApplication::~IApplication()
{
		
}

void IApplication::Initialize(HWND hwnd, UINT width, UINT height)
{
	AdjustWindowRect(hwnd, width, height);
	InitializeEtc();

	mGraphicDevice = make_unique<RGraphicDevice_DX11>();
	mGraphicDevice->Initialize();

	JFmod::Initialize();
	JCollisionManager::Initialize();
	UUIManager::Initialize();
	JSceneManager::Initialize();
}

void IApplication::AdjustWindowRect(HWND hwnd, UINT width, UINT height)
{
	mHwnd = hwnd;
	mHdc = GetDC(hwnd);

	RECT rect = { 0, 0, (LONG)width, (LONG)height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	mWidth = rect.right - rect.left;
	mHeight = rect.bottom - rect.top;

	SetWindowPos(hwnd, nullptr, 0, 0, mWidth, mHeight, 0);
	ShowWindow(hwnd, true);
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
	Render();

	Destroy();
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
