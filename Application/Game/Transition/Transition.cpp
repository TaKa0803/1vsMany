#include "Transition.h"
#include"TextureManager/TextureManager.h"
#include"DeltaTimer/DeltaTimer.h"

Transition::Transition()
{
	int texture = TextureManager::white_;
	transitionSprite_.reset(Sprite::Create(texture, { 1,1 }, { 1,1 }, { 1280,720 }));
	transitionSprite_->SetMaterialDataColor({ 0,0,0,1 });

}

bool Transition::Update()
{

	currentSceneXhangeSec_ += (float)DeltaTimer::deltaTime_;

	//カウントが指定地を超えた場合
	if (currentSceneXhangeSec_ >= maxSceneChangeSec_) {
		return true;
	}

	((this->*Transition::TypeUpdate[(int)type_])());


	return false;

}

void Transition::Draw()
{
	transitionSprite_->Draw();
}


//更新
void (Transition::* Transition::TypeUpdate[])() {
	&Transition::Black2ClearUpdate,
	& Transition::Clear2BlackUpdate
};


void Transition::SetAndStartTransition(TransitionType type)
{
	type_ = type;
	//時間をリセット
	currentSceneXhangeSec_ = 0;
}

void Transition::Black2ClearUpdate()
{
	//透明度計算
	float alpha =1.0f- float(currentSceneXhangeSec_ / maxSceneChangeSec_);

	if (alpha < 0) {
		alpha = 0;
	}

	//透明度を反映
	transitionSprite_->SetColorAlpha(alpha);
}

void Transition::Clear2BlackUpdate()
{
	//割合から透明度算出
	float alpha = float(currentSceneXhangeSec_ / maxSceneChangeSec_);
	
	if (alpha > 1.0f) {
		alpha = 1.0f;
	}
	//透明度を反映
	transitionSprite_->SetColorAlpha(alpha);
}
