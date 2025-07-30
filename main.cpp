#include"base/memoryReakChecker.h"
#include"MainSystem/MainSystem.h"
#include"SingleGameObjects/Model.h"
#include<dxgidebug.h>


LPCWSTR WindowApp::windowName_ = L"あ";

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	RegisterLeakCheck();

	MainSystem* engine = new MainSystem();

	engine->Run();

	delete engine;

	return 0;
}