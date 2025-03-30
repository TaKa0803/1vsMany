#include "PlayerBButtonComboManager.h"
#include"Game/Player/Player.h"

#include"Game/Player/Behavior/AttackManager/Attacks/Punch/PlayerAttackPunch.h"
#include"Game/Player/Behavior/AttackManager/Attacks/Kick/PlayerAttackKick.h"
#include"Game/Player/Behavior/AttackManager/Attacks/Drill/PlayerAttackDrill.h"

PlayerBButtonManager::PlayerBButtonManager(Player* player)
{
	//プレイヤーポインタをセット
	player_ = player;

	//攻撃データをセット
	attacks_.resize((size_t)AttackType::CountAttackType);
	attacks_[(size_t)AttackType::Punch] =std::make_unique<PlayerAttackPunch>();
	attacks_[(size_t)AttackType::Kick] = std::make_unique<PlayerAttackKick>();
	attacks_[(size_t)AttackType::Drill] = std::make_unique<PlayerAttackDrill>();
	
	//デバッグ用にパラメータをツリーにセット
	tree_.name_ = "Bボタン攻撃";
	//デバッグツリーをセット
	for (int i = 0; i < (int)AttackType::CountAttackType; i++) {
		//ツリーをセット
		tree_.SetTreeData(attacks_[i]->tree_);
	}
}

void PlayerBButtonManager::Init()
{
	//攻撃タイプをセット
	type_ = (AttackType)(player_->baseParameter_.comboCount);

	//攻撃データを初期化
	attacks_[(size_t)type_]->Init();
}

void PlayerBButtonManager::Update()
{
	//攻撃データを更新
	attacks_[(size_t)type_]->Update();
}
