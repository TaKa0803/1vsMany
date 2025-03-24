#pragma once
#include"Game/Player/Behavior/PlayerBaseBehavior.h"
#include"Game/Player/Behavior/AttackManager/PlayerBaseButtonManager.h"
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

	//入力の種類
	enum class InputType {
		A,				//Aボタン
		B,				//Bボタン		
		CountInputType	//入力の数
	};

	//ボタンごとの攻撃管理
	std::vector<std::unique_ptr<PlayerBaseButtonManager>>buttonManagers_;

};