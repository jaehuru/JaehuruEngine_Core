#include "HighLevelInterface/JApplication.h"
#include "Renderer/RRenderer.h"
#include "Helpers/Input.h"
#include "Helpers/Time.h"
#include "Scene/JSceneManager.h"
#include "Resource/RResources.h"
#include "Collision/JCollisionManager.h"
#include "UI/UUIManager.h"
#include "FMOD/JFmod.h"


JApplication::JApplication() : 
	mbLoaded(false),
	mHwnd(nullptr),
	mHdc(nullptr),
	mWidth(0),
	mHeight(0),
	mBackHdc(NULL),
	mBackBitmap(NULL)
{

}

JApplication::~JApplication()
{
		
}

void JApplication::Initialize(HWND hwnd, UINT width, UINT height)
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

void JApplication::AdjustWindowRect(HWND hwnd, UINT width, UINT height)
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

void JApplication::InitializeEtc()
{
	Input::Initialize();
	Time::Initialize();
}

void JApplication::Run()
{
	if (mbLoaded == false)
		mbLoaded = true;

	Update();
	LateUpdate();
	Render();

	Destroy();
}

void JApplication::Update()
{
	Input::Update();
	Time::Update();
	JCollisionManager::Update();
	UUIManager::Update();
	JSceneManager::Update();
}

void JApplication::LateUpdate()
{
	JCollisionManager::LateUpdate();
	UUIManager::LateUpdate();
	JSceneManager::LateUpdate();
}

void JApplication::Render()
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

void JApplication::Destroy()
{
	JSceneManager::Destroy();
}

void JApplication::Release()
{
	JSceneManager::Release();
	UUIManager::Release();
	RResources::Release();
	Release();
}
