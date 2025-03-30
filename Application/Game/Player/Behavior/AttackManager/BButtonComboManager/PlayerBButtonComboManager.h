#pragma once
#include"Game/Player/Behavior/AttackManager/PlayerBaseButtonManager.h"
#include"Game/Player/Behavior/AttackManager/Attacks/PlayerBaseAttack.h"
#include<vector>
#include<memory>

//前方宣言
class Player;

//プレイヤーのBボタンでの攻撃操作
class PlayerBButtonManager:public PlayerBaseButtonManager{

public://**パブリック関数**//
	PlayerBButtonManager(Player*player);
	~PlayerBButtonManager()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	//攻撃タイプ
	enum class AttackType{
		Punch,			//パンチ
		Kick,			//キック
		Drill,			//ドリル	
		CountAttackType	//攻撃の数
	};

	//攻撃データ群
	std::vector<std::unique_ptr<PlayerBaseAttack>>attacks_;

private://**プライベート変数**//

	//プレイヤーポインタ
	const Player* player_ = nullptr;

	//攻撃タイプ
	AttackType type_ = AttackType::Punch;

};