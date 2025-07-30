#pragma once

#include<Windows.h>

//ウィンドウ関係の処理クラス
class WindowApp {
public://

	WindowApp()=default;
	~WindowApp();

public://静的メンバ変数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//ウィンドウ名
	static LPCWSTR windowName_;

	//画面サイズ
	static int kClientWidth_;
	static int kClientHeight_;

public://パブリック関数**//

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// プロセスメッセージ
	/// </summary>
	bool ProcessMessage();

	/// <summary>
	/// hwnd取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd()const { return hwnd_; }

	/// <summary>
	/// wc取得
	/// </summary>
	/// <returns></returns>
	WNDCLASS GetWc()const { return wc; }

private://**プライベート変数**//

	WNDCLASS wc{};
	//ウィンドウハンドル
	HWND hwnd_ = nullptr;
};