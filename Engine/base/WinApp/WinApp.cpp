#include"WinApp.h"
#include"Log/Log.h"

#include<d3d12.h>
#pragma region ImGui
#include<imgui.h>
#include<imgui_impl_dx12.h>
#include<imgui_impl_win32.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion



int WindowApp::kClientWidth_ = 1280;
int WindowApp::kClientHeight_ = 720;

WindowApp::~WindowApp()
{
	CloseWindow(hwnd_);
}

//ウィンドウプロシーシャ
LRESULT CALLBACK WindowApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	//メッセージに対してゲーム固有の処理をする
	switch (msg)
	{
	case WM_DESTROY:PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WindowApp::Initialize()
{
#pragma region ウィンドウ生成 

	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"A";
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&wc);

	
	RECT wrc = { 0,0,kClientWidth_,kClientHeight_ };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(
		wc.lpszClassName,
		windowName_,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr);

	

	ShowWindow(hwnd_, SW_SHOW);
#pragma endregion
	Log("Complete WinAppInitialize\n");
}

bool WindowApp::ProcessMessage()
{
	MSG msg{};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message != WM_QUIT) {

		return true;
	}
	else {
		return false;
	}
}
