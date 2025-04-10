#include"SceneManager/SceneManager.h"
#include"GlobalVariable/Manager/GlobalVaribleManager.h"
#include"ImGuiManager/ImGuiManager.h"

#include"SceneFactory/SceneFactory.h"


#include<cassert>


SceneManager::SceneManager(){}

void SceneManager::Initialize()
{
	//初期シーン設定
	IScene::SetScene(SCENE::TITLE);
}

void SceneManager::Update()
{
	//デバッグウィンドウ処理
	Debug();

	//シーンチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = static_cast<int>(IScene::GetSceneNo());

	//シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_)
	{
		//変更していたら		
		//過去のデバッグデータをクリア
		GlobalVariableManager::GetInstance()->ClearSetData();

		//シーンを読み込み
		scene_ = SceneFactory::CreateScene((SCENE)currentSceneNo_);

		//保存したパラメータをセット
		GvariM::GetInstance()->SetLoadAllData();

		//シーンの初期化
		scene_->Initialize();
	}

	//デバッグ用に現シーン名に変更
	scenename_ = scene_->name_;
	
	//シーン更新処理
	scene_->Update();
}

void SceneManager::Draw()
{	
	//描画処理
	scene_->Draw();
}

void SceneManager::Debug()
{
#ifdef _DEBUG
	ImGui::Begin("シーンマネージャー");
	ImGui::SliderInt("シーン番号", &IScene::GetSceneNo(),0,(int)SCENE::SceneCount-1);
	ImGui::Text("シーン名 : %s", scenename_.c_str());
	ImGui::Text("フレームレート : %4.1f", ImGui::GetIO().Framerate);
	ImGui::End();
#endif // _DEBUG

}



