#include"TitleScene.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"

TitleScene::TitleScene() {

	//入力のインスタンス取得
	input_ = Input::GetInstance();

	//UIのインスタンス生成
	uis_ = std::make_unique<TitleUIs>(select_);

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
}

void TitleScene::Update() {


	Debug();

	if (!isSceneChange_) {
		float stickX = input_->GetjoyStickL().x;

		if (input_->TriggerKey(DIK_A) || stickX < -0.7f) {
			select_ = TitleSelect2Input::Start;
		}
		else if (input_->TriggerKey(DIK_D) || stickX > 0.7f) {
			select_ = TitleSelect2Input::Leave;
		}
	}

	uis_->Update();

	SceneChange();

}

void TitleScene::Draw() {

	uis_->Draw();

	sceneC_->Draw();

	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kLightOutline, true);


}

void TitleScene::Debug() {

	//Vector3 pos = pressSp_->GetPosition();
	//Vector3 scale = pressSp_->GetScale();

//#ifdef _DEBUG
//	ImGui::Begin("sprite");
//	ImGui::DragFloat3("pos", &pos.x);
//	ImGui::DragFloat3("scale", &scale.x);
//	ImGui::End();
//#endif // _DEBUG
//
//	pressSp_->SetPosition(pos);
//	pressSp_->SetScale(scale);

}

void TitleScene::SceneChange()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		isSceneChange_ = true;
	}

	if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
		isSceneChange_ = true;
	}

	if (input_->TriggerKey(DIK_ESCAPE)) {
		leaveGame = true;
	}


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
