#include "CircleShadow.h"

CircleShadow::CircleShadow(const EulerWorldTransform& world)
{
	//初期化
	Initialize("DZone");
	//親ワールドを設定
	SetParent(&world);
	//色を黒色に設定
	SetColor({ 0,0,0,1 });
	//高さを少し上げる
	world_.translate_ = { 0,0.1f,0 };
	//サイズ設定
	SetScale(1.5f);
}

void CircleShadow::Draw()
{
	//更新
	Update();
	//影の描画
	Draw();
}
