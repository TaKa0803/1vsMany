#pragma once
#include"Game/Player/Behavior/PlayerBaseBehavior.h"
#include"Game/Player/Behavior/AttackManager/PlayerBaseButtonManager.h"
#include"Game/Player/Behavior/AttackManager/Attacks/PlayerBaseAttack.h"

#include<vector>

//プレイヤーの攻撃管理クラス
class PlayerAttackManager :public PlayerBaseBehavior {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAttackManager();
	~PlayerAttackManager() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private://**プライベート変数**//

	//攻撃コンボの最大数
	const int attackMax_ = 3;

	//入力の種類
	enum class InputType {
		B,				//Bボタン	
		A,				//Aボタン
		CountInputType	//入力の数
	}type_ = InputType::B;
		
	//タイプリクエスト
	std::optional<InputType>typeRequest_ = std::nullopt;

	//ボタンごとの攻撃管理
	std::vector<std::unique_ptr<PlayerBaseButtonManager>>buttonManagers_;

	//Aボタンの入力
	bool isInputA_ = false;
	//Bボタンの入力
	bool isInputB_ = false;

};