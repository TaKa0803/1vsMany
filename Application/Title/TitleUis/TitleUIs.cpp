#include "TitleUIs.h"
#include"TextureManager/TextureManager.h"
#include"DeltaTimer/DeltaTimer.h"

TitleUIs::TitleUIs(TitleSelect2Input& select)
{
	//ポインタ取得
	select_ = &select;

	int texture = TextureManager::LoadTex("resources/Texture/AL/Title.png");
	titleText_.reset(Sprite::Create(texture, { 320,180 }, { 320,180 }, { 1280,720 }));

	texture = TextureManager::LoadTex("resources/Texture/Game/dengen.png");
	dengenIcon_.reset(Sprite::Create(texture, { 160,90 }, { 160,90 }, { 100,70 }));

	texture = TextureManager::LoadTex("resources/Texture/Game/busterApp.png");
	startIcon_.reset(Sprite::Create(texture, { 160,90 }, { 160,90 }, { 100,70 }));

	texture = TextureManager::LoadTex("resources/Texture/Game/windows_tab.png");
	windowTab_.reset(Sprite::Create(texture, { 1280,720 }, { 1280,720 }, { 1280,720 }));
	
	texture = TextureManager::LoadTex("resources/Texture/Game/selectBox.png");
	selectWindow_.reset(Sprite::Create(texture, { 64,64 }, { 64,64 }, { 64,64 }));

	texture = TextureManager::LoadTex("resources/Texture/Game/text_hajimeru.png");
	text_hajimeru_.reset(Sprite::Create(texture, { 64,32 }, { 64,32 }, { 64,32 }));

	texture = TextureManager::LoadTex("resources/Texture/Game/text_owaru.png");
	text_owaru_.reset(Sprite::Create(texture, { 64,32 }, { 64,32 }, { 64,32 }));

#pragma region デバッグ用の値設定
	tree_.name_ = "UIスプライト";
	tree_.SetTreeData(titleText_->GetTree("タイトルと背景"));
	tree_.SetTreeData(startIcon_->GetTree("スタートアイコン"));
	tree_.SetTreeData(dengenIcon_->GetTree("電源アイコン"));
	tree_.SetTreeData(windowTab_->GetTree("ウィンドウタブ"));
	tree_.SetTreeData(selectWindow_->GetTree("選択ウィンドウ"));
	tree_.SetTreeData(text_hajimeru_->GetTree("始める"));
	tree_.SetTreeData(text_owaru_->GetTree("終わる"));
#pragma endregion
}

void TitleUIs::Initialize()
{
}

void TitleUIs::Update()
{
	//点滅の更新
	BlinkingUpdate();

	//選択によってスプライト座標変更
	SelectPositionUpdate();

	

}

void TitleUIs::DrawBack()
{
	titleText_->Draw();
}

void TitleUIs::DrawUI()
{
	//ウィンドウタブ描画
	windowTab_->Draw();

	selectWindow_->Draw();

	//アイコン描画
	startIcon_->Draw();
	dengenIcon_->Draw();


	//テキスト描画
	if (*select_ == TitleSelect2Input::Start) {
		text_hajimeru_->Draw();
	}
	else if (*select_ == TitleSelect2Input::Leave) {
		text_owaru_->Draw();
	}
}

void TitleUIs::BlinkingUpdate()
{
	//反転フラグがOFFの場合
	if (!Inverse_) {
		//時間を加算
		blinkingSec_ += (float)DeltaTimer::deltaTime_;
		//最大時間を超えたら反転フラグをONにする
		if (blinkingSec_ >= maxBlinkingSec_) {
			//時間を最大時間にセット
			blinkingSec_ = maxBlinkingSec_;
			//反転フラグをONにする
			Inverse_ = true;
		}
	}
	else {
		//反転フラグがONの場合は時間を減算
		blinkingSec_ -= (float)DeltaTimer::deltaTime_;
		//時間が0以下になったら反転フラグをOFFにする
		if (blinkingSec_ <= 0) {
			//時間を0にセット
			blinkingSec_ = 0;
			//反転フラグをOFFにする
			Inverse_ = false;
		}
	}

	//進行割合取得
	float t = blinkingSec_ / maxBlinkingSec_;
	//透明度の設定
	selectWindow_->SetColorAlpha(t);
}

void TitleUIs::SelectPositionUpdate()
{
	///選択物で座標位置変更
	//スタートアイコンを選択している場合
	if (*select_ == TitleSelect2Input::Start) {
		//アイコンの透明度を変更
		startIcon_->SetColorAlpha(1);
		//電源アイコンの透明度を変更
		dengenIcon_->SetColorAlpha(0.5f);

		//座標合わせる
		selectWindow_->SetPosition(startIcon_->GetPosition());
		//サイズを変更
		selectWindow_->SetScale(startIcon_->GetScale() * 1.2f);
	}
	else if (*select_ == TitleSelect2Input::Leave) {
		//電源アイコンを選択している場合
		//アイコンの透明度を変更
		startIcon_->SetColorAlpha(0.5f);
		//電源アイコンの透明度を変更
		dengenIcon_->SetColorAlpha(1);

		//座標合わせる
		selectWindow_->SetPosition(dengenIcon_->GetPosition());
		//サイズを変更
		selectWindow_->SetScale(dengenIcon_->GetScale() * 1.2f);
	}
}
