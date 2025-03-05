#include "PlayerAnimationManager.h"

PlayerAnimationManager::PlayerAnimationManager(Model* model)
{
	//モデルデータ参照
	model_ = model;
}

void PlayerAnimationManager::SetAnimation(Animation anime)
{
	//配列番号取得
	size_t num = (size_t)anime;
	//アニメーション設定を反映
	model_->ChangeAnimation(animeName_[anime], transitionSeconds_[num]);
	model_->SetAnimationRoop(isLoops_[num]);
	model_->animationRoopSecond_ = multiplyAnimationSpd_[num];
}
