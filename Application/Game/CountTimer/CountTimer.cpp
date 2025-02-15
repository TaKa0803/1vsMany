#include "CountTimer.h"
#include"TextureManager/TextureManager.h"
#include"DeltaTimer/DeltaTimer.h"

CountTimer::CountTimer()
{

	int texture = TextureManager::LoadTex("resources/Texture/AL/number64x90.png");
	num1_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 680,90 }));
	num10_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 610,90 }));


	num1_->SetPosition({ 680,90 });
	num1_->SetScale({ 90,90 });

	num10_->SetPosition({ 610,90 });
	num10_->SetScale({ 90,90 });

	currentCount_ += param_.maxCount;
}

void CountTimer::Update()
{

	currentCount_ -= (float)DeltaTimer::deltaTime_;

	if (currentCount_ <= 0) {
		currentCount_ = 0;
		isCountEnd_ = true;
	}

	//分に変換
	int minute = (int)currentCount_;

	int second = (int)minute % 10;

	num1_->SetTVTranslate({ ((float)second / 10.0f) - 0.1f, 0 });

	//二桁目のみの情報取得
	minute = minute - second;

	int minute2 = minute / 10;

	num10_->SetTVTranslate({ ((float)minute2 / 10.0f) - 0.1f,0 });
}

void CountTimer::Draw()
{
	num1_->Draw();
	num10_->Draw();
}
