#include "PlayerAttackPunch.h"
#include"Game/Player/Player.h"

PlayerAttackPunch::PlayerAttackPunch()
{
	tree_.name_ = "パンチ攻撃";
}

void PlayerAttackPunch::InitReserve()
{
	//アニメーションをセット
	player_->SetAnimation(PlayerAnimationManager::ATK_Punch);
}

void PlayerAttackPunch::InitExecution()
{
	//音の再生
	player_->SetPlaySound(PlayerSoundManager::Punch);
}

void PlayerAttackPunch::InitRigor()
{
	//音の停止
	//player_->SetStopSound(PlayerSoundManager::Punch);
}

void PlayerAttackPunch::UpdateReserve()
{
	//入力方向に向く
	player_->SetBody4Input();

	//条件を満たしたら次の状態へ
	if (currentSec_ >= nowMaxSec_) {
		//次の状態へ
		behaviorRequest_ = Behavior::Execution;
	}
}

void PlayerAttackPunch::UpdateExecution()
{
	//入力方向に向いて移動
	SetInput2Move();
	
	//条件を満たしたら次の状態へ
	if (currentSec_ >= nowMaxSec_) {
		//次の状態へ
		behaviorRequest_ = Behavior::Rigor;
	}
}

void PlayerAttackPunch::UpdateRigor()
{
	//条件を満たしたらおわる
	if (currentSec_ >= nowMaxSec_) {
		//おわる
		isEnd_ = true;
	}
}
