#include "CountTimer.h"
#include"TextureManager/TextureManager.h"
#include"DeltaTimer/DeltaTimer.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

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

	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("時間カウント");
	gvg->SetMonitorValue("残り時間", &currentCount_);
	gvg->SetMonitorValue("カウント停止", &isStopCount_);
	gvg->SetValue("制限時間", &param_.maxCount);

	gvg->SetTreeData(num1_->GetTree("1の位"));
	gvg->SetTreeData(num10_->GetTree("10の位"));
}

void CountTimer::GameUpdate()
{
	if (!isStopCount_) {
		//時間を減少
		currentCount_ -= (float)DeltaTimer::deltaTime_;

		//カウントが0以下
		if (currentCount_ <= 0) {
			currentCount_ = 0;
			//フラグをON
			isCountEnd_ = true;
		}
	}
}

void CountTimer::SpriteUpdate()
{
	//分に変換
	int minute = (int)currentCount_;

	int second = (int)minute % 10;

	num1_->SetUVTranslate({ ((float)second / 10.0f) - 0.1f, 0 });

	//二桁目のみの情報取得
	minute = minute - second;

	int minute2 = minute / 10;

	num10_->SetUVTranslate({ ((float)minute2 / 10.0f) - 0.1f,0 });

}

void CountTimer::Draw()
{
	num1_->Draw();
	num10_->Draw();
}
