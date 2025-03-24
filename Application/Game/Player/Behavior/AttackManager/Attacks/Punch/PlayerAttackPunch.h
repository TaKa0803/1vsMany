#pragma once
#include"Game/Player/Behavior/AttackManager/Attacks/PlayerBaseAttack.h"

//プレイヤーのパンチ攻撃
class PlayerAttackPunch :public PlayerBaseAttack {

public://**パブリック関数**//

	PlayerAttackPunch();
	~PlayerAttackPunch() = default;

	//各状態の初期化
	void InitReserve()override ;
	void InitExecution()override;
	void InitRigor()override;

	//各状態更新巻数
	void UpdateReserve() override;
	void UpdateExecution() override;
	void UpdateRigor() override;
};