#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

//丸い影
class CircleShadow : public InstancingGameObject {

public://**パブリック関数**//

	CircleShadow(const EulerWorldTransform&world);
	~CircleShadow() = default;
};