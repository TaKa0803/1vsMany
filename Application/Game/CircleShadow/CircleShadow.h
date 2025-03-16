#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

//丸い影
class CircleShadow : public InstancingGameObject {

public://**パブリック関数**//

	CircleShadow(const EulerWorldTransform&world);
	~CircleShadow() = default;

	void Update()override;

private://**プライベート変数**//

	//追従元のワールドポインタ
	const EulerWorldTransform* parents_;

	//高さ
	float height_ = 0.1f;

};