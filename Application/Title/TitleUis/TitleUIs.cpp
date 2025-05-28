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


	tree_.name_ = "UIスプライト";
	tree_.SetTreeData(titleText_->GetTree("タイトルと背景"));
	tree_.SetTreeData(startIcon_->GetTree("スタートアイコン"));
	tree_.SetTreeData(dengenIcon_->GetTree("電源アイコン"));
	tree_.SetTreeData(windowTab_->GetTree("ウィンドウタブ"));
	tree_.SetTreeData(selectWindow_->GetTree("選択ウィンドウ"));
	tree_.SetTreeData(text_hajimeru_->GetTree("始める"));
	tree_.SetTreeData(text_owaru_->GetTree("終わる"));
}

void TitleUIs::Initialize()
{
}

void TitleUIs::Update()
{

	if (!Inverse_) {
		tenmetuSec_ += (float)DeltaTimer::deltaTime_;
		if (tenmetuSec_ >= maxTermetuSec_) {
			tenmetuSec_ = maxTermetuSec_;
			Inverse_ = true;
		}
	}
	else {
		tenmetuSec_ -= (float)DeltaTimer::deltaTime_;
		if (tenmetuSec_ <= 0) {
			tenmetuSec_ = 0;
			Inverse_ = false;
		}
	}

	//点滅処理
	float t = tenmetuSec_ / maxTermetuSec_;
	//透明度の設定
	selectWindow_->SetColorAlpha(t);

	//選択物で座標位置変更
	if (*select_ == TitleSelect2Input::Start) {
		startIcon_->SetColorAlpha(1);
		dengenIcon_->SetColorAlpha(0.5f);

		//座標合わせる
		selectWindow_->SetPosition(startIcon_->GetPosition());
		selectWindow_->SetScale(startIcon_->GetScale() * 1.2f);
	}
	else if (*select_ == TitleSelect2Input::Leave) {
		startIcon_->SetColorAlpha(0.5f);
		dengenIcon_->SetColorAlpha(1);

		//座標合わせる
		selectWindow_->SetPosition(dengenIcon_->GetPosition());
		selectWindow_->SetScale(dengenIcon_->GetScale()*1.2f);
	}

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