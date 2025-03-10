#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

//丸い影
class CircleShadow : private InstancingGameObject {

public://**パブリック関数**//

	CircleShadow(const EulerWorldTransform&world);
	~CircleShadow() = default;

	//描画
	void Draw();
};