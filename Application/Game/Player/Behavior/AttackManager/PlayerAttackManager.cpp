#include "PlayerAttackManager.h"
#include"Game/Player/Player.h"

#include"Game/Player/Behavior/AttackManager/BButtonComboManager/PlayerBButtonComboManager.h"

PlayerAttackManager::PlayerAttackManager()
{

	//各ボタンの攻撃管理クラスの配列のサイズをセット
	buttonManagers_.resize((size_t)InputType::CountInputType);

	//各マネージャをセット
	buttonManagers_[(size_t)InputType::B] = std::make_unique<PlayerBButtonManager>(player_);
}

void PlayerAttackManager::Init()
{
    //コンボ数を初期化
	player_->parameter_.comboCount = 0;
	//移動量を初期化
	player_->parameter_.velocity.SetZero();
}

void PlayerAttackManager::Update()
{
}
