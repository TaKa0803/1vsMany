#include"PlayerBaseBehavior.h"


//データ初期化
std::optional<State>PlayerBaseBehavior::behaviorRequest_ = std::nullopt;

Player* PlayerBaseBehavior::player_ = nullptr;

PlayerBaseBehavior::PlayerBaseBehavior()
{
	if (!input_) {
		input_ = std::make_unique<PlayerInputManager>();
	}

	if (!camera_) {
		camera_ = Camera::GetInstance();
	}
}
