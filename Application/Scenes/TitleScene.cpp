#include"TitleScene.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"
#include"Camera/Camera.h"

TitleScene::TitleScene() {

	//入力のインスタンス取得
	input_ = Input::GetInstance();

	//UIのインスタンス生成
	uis_ = std::make_unique<TitleUIs>(select_);

	//ジャンプ敵の生成
	jumpEnemyManager_ = std::make_unique<JumpEnemyManager>();

	//遷移テクスチャ
	int texture = TextureManager::LoadTex(white);
	sceneC_.reset(Sprite::Create(texture, { 1,1 }, { 1,1 }, { 1280,720 }));
	sceneC_->SetMaterialDataColor({ 0,0,0,1 });

	//タイトルBGMの配列番号取得
	titleSound_ = AudioManager::LoadSoundNum("title");

	//デバッグ用にパラメータを設定
	std::unique_ptr<GVariGroup> gvg = std::make_unique<GVariGroup>("Title");
	gvg->SetTreeData(uis_->GetTree());

}

void TitleScene::Initialize() {
	sceneXhangeCount_ = maxSceneChangeCount_;
	isSceneChange_ = false;
	preSceneChange_ = false;
	sceneC_->SetColorAlpha(1);

	uis_->Initialize();

	//初期状態
	select_ = TitleSelect2Input::Start;

	AudioManager::GetInstance()->StopAllSounds();
	AudioManager::PlaySoundData(titleSound_, 0.08f);

	Camera::GetInstance()->Initialize();
	
}

void TitleScene::Update() {

	//カメラ更新
	Camera::GetInstance()->Update();

	//入力更新
	if (!isSceneChange_) {
		float stickX = input_->GetjoyStickL().x;

		if (input_->TriggerKey(DIK_A) || stickX < -0.7f) {
			select_ = TitleSelect2Input::Start;
		}
		else if (input_->TriggerKey(DIK_D) || stickX > 0.7f) {
			select_ = TitleSelect2Input::Leave;
		}
	}

	//ジャンプしている敵の更新
	jumpEnemyManager_->Update();

	//UIの更新
	uis_->Update();

	//シーン変更処理
	SceneChange();

}

void TitleScene::Draw() {

	//背景描画
	uis_->DrawBack();

	//ジャンプする敵の描画
	jumpEnemyManager_->Draw();

	//Instancingモデル描画
	InstancingModelManager::GetInstance()->DrawAllModel();

	//UI描画
	uis_->DrawUI();

	//遷移画像描画
	sceneC_->Draw();

	//ポストエフェクトの描画
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kLightOutline, true);
}

void TitleScene::SceneChange()
{
	//入力で遷移開始
	if (input_->TriggerKey(DIK_SPACE)) {
		isSceneChange_ = true;
	}
	//コントローラー入力で遷移開始
	if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
		isSceneChange_ = true;
	}
	//入力でゲーム終了
	if (input_->TriggerKey(DIK_ESCAPE)) {
		leaveGame = true;
	}

	//遷移処理
	if (isSceneChange_) {

		//シーン変更時選択物
		if (select_ == TitleSelect2Input::Leave) {
			leaveGame = true;
			return;
		}

		float alpha = float(sceneXhangeCount_ / maxSceneChangeCount_);

		sceneC_->SetColorAlpha(alpha);

		if (sceneXhangeCount_++ >= maxSceneChangeCount_) {
			sceneC_->SetColorAlpha(1);
			sceneNo = (int)SCENE::GAME;
		}
	}
	else {

		if (!preSceneChange_) {

			float alpha = float(sceneXhangeCount_ / maxSceneChangeCount_);

			sceneC_->SetColorAlpha(alpha);

			if (sceneXhangeCount_-- <= 0) {
				sceneXhangeCount_ = 0;
				sceneC_->SetColorAlpha(0);
				preSceneChange_ = true;
			}

		}
	}
}
