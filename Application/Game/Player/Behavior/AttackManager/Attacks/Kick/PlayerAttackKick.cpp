#include "PlayerAttackKick.h"
#include"Game/Player/Player.h"

PlayerAttackKick::PlayerAttackKick()
{
	tree_.name_ = "キック攻撃";
}

void PlayerAttackKick::InitReserve()
{
	//アニメーションをセット
	player_->SetAnimation(PlayerAnimationManager::ATK_Kick);
}

void PlayerAttackKick::InitExecution()
{
	//音の再生
	player_->SetPlaySound(PlayerSoundManager::Kick);
}

void PlayerAttackKick::InitRigor()
{
	//音の停止
	player_->SetStopSound(PlayerSoundManager::Punch);
}

void PlayerAttackKick::UpdateReserve()
{
	//入力方向に向く
	player_->SetBody4Input();

	//条件を満たしたら次の状態へ
	if (currentSec_ >= nowMaxSec_) {
		//次の状態へ
		behaviorRequest_ = Behavior::Execution;
	}
}

void PlayerAttackKick::UpdateExecution()
{
	//入力方向に向いて移動
	SetInput2Move();

	//条件を満たしたら次の状態へ
	if (currentSec_ >= nowMaxSec_) {
		//次の状態へ
		behaviorRequest_ = Behavior::Rigor;
	}
}

void PlayerAttackKick::UpdateRigor()
{
	//条件を満たしたらおわる
	if (currentSec_ >= nowMaxSec_) {
		//おわる
		attackData_.isEnd = true;
	}
}
