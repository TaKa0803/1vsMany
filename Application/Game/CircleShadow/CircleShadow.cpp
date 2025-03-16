#include "CircleShadow.h"

CircleShadow::CircleShadow(const EulerWorldTransform& world)
{
	//初期化
	Initialize("DZone");
	//親ワールドを設定
	parents_ = &world;
	//色を黒色に設定
	SetColor({ 0,0,0,1 });
	//サイズ設定
	SetScale(1.5f);
}

void CircleShadow::Update()
{
	//追従元の座標に合わせる
	world_.translate_ = parents_->GetWorldTranslate();
	//高さのみ固定
	world_.translate_.y = height_;
	//行列更新
	world_.UpdateMatrix();
}


