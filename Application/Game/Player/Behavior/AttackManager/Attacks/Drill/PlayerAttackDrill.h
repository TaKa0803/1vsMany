#pragma once
#include"Game/Player/Behavior/AttackManager/Attacks/PlayerBaseAttack.h"

//プレイヤーのドリル攻撃
class PlayerAttackDrill :public PlayerBaseAttack {

public://**パブリック関数**//

	PlayerAttackDrill();
	~PlayerAttackDrill() = default;

	//各状態の初期化
	void InitReserve()override;
	void InitExecution()override;
	void InitRigor()override;

	//各状態更新巻数
	void UpdateReserve() override;
	void UpdateExecution() override;
	void UpdateRigor() override;

};