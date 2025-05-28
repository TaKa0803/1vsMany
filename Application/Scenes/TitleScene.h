#pragma once
#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Title/TitleUis/TitleUIs.h"
#include"Title/JumpEnemyManager/JumpEnemyManager.h"

class TitleScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	~TitleScene()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private://**プライベート関数**//


	//シーン変更処理
	void SceneChange();

private:
	//キー入力
	Input* input_ = nullptr;

	//UI
	std::unique_ptr<TitleUIs>uis_;

	//跳ねる敵のマネージャ
	std::unique_ptr<JumpEnemyManager>jumpEnemyManager_;

	TitleSelect2Input select_ = TitleSelect2Input::Start;

	//シーンチェンジ用
	std::string white = "resources/Texture/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;

	//シーン転換後の処置
	bool preSceneChange_ = false;

	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	//シーン転換カウント
	const float maxSceneChangeCount_ = 60;

	float sceneXhangeCount_ = 0;

	//タイトルBGM
	int titleSound_;

};