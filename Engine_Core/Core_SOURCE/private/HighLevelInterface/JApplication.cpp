#include "HighLevelInterface/JApplication.h"
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
#include "Event/JApplicationEvent.h"
#include "Event/JMouseEvent.h"

JApplication::JApplication() : 
	bLoaded(false),
	bRunning(false),
	bMinimized(false)
{
	mWindow.SetEventCallBack(J_BIND_EVENT_FN(JApplication::OnEvent));
}

JApplication::~JApplication()
{
		
}

void JApplication::Initialize(HWND hwnd, int width, int height)
{
	mWindow.SetHwnd(hwnd);
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

void JApplication::InitializeWindow(HWND hwnd)
{
	SetWindowPos(hwnd, nullptr, mWindow.GetXPos(), mWindow.GetYPos(),
					mWindow.GetWindowWidth(), mWindow.GetWindowHeight(), 0);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

void JApplication::AdjustWindowRect(HWND hwnd, int width, int height)
{
	RECT rect = { 0, 0, (LONG)width, (LONG)height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	RECT winRect;
	::GetWindowRect(mWindow.GetHwnd(), &winRect);

	//window position
	mWindow.SetPos(winRect.left, winRect.top);

	// window size
	mWindow.SetWindowWidth(rect.right - rect.left);
	mWindow.SetWindowHeight(rect.bottom - rect.top);

	//client size
	mWindow.SetWidth(width);
	mWindow.SetHeight(height);

	InitializeWindow(hwnd);
}

void JApplication::ReszieGraphicDevice(UINT width, UINT height)
{
	if (mGraphicDevice == nullptr)
		return;

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	mWindow.SetWidth(viewport.Width);
	mWindow.SetHeight(viewport.Height);

	mGraphicDevice->Resize(viewport);
	renderer::FrameBuffer->Resize(viewport.Width, viewport.Height);
}

void JApplication::InitializeEtc()
{
	Input::Initialize();
	Time::Initialize();
}

void JApplication::OnEvent(IEvent& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) -> bool
		{
			ReszieGraphicDevice(e.GetWidth(), e.GetHeight());
			return true;
		});

	dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) -> bool
		{
			// Todo : MouseMovedEvent
			return true;
		});
}

void JApplication::Run()
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

void JApplication::Close()
{
	bRunning = false;
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

	//copy back buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> src = GetDevice()->GetFrameBuffer();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> dst = renderer::FrameBuffer->GetAttachmentTexture(0)->GetTexture();

	GetDevice()->CopyResource(dst.Get(), src.Get());
}

void JApplication::Present()
{
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
	renderer::Release();
}
