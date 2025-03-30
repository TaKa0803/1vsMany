#include"PlayerBaseBehavior.h"


//データ初期化
std::optional<State>PlayerBaseBehavior::behaviorRequest_ = std::nullopt;

Player* PlayerBaseBehavior::player_ = nullptr;

PlayerBaseBehavior::PlayerBaseBehavior()
{
	//カメラのインスタンス未取得の場合取得
	if (!camera_) {
		camera_ = Camera::GetInstance();
	}
}
