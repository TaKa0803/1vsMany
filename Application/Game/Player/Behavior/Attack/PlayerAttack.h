#pragma once
#include"Game/Player/Behavior/PlayerBaseBehavior.h"

//攻撃のデータ構造体
struct ATKData {
	float extraTime = 1.0f;	//予備動作
	float AttackTime = 1.0f;	//攻撃動作
	float RigorTime = 1.0f;	//硬直時間

	int isYATK = false;	//Yボタン攻撃か

	float spd = 1.0f;

	std::vector<ATKData>ATKDerivation;	//攻撃の派生
};

class PlayerAttack :public PlayerBaseBehavior {

public://**パブリック関数**//

	PlayerAttack();
	~PlayerAttack() = default;

	void Init()override;

	void Update()override;

private://**プライベート変数



	//ボタンを押したときからの攻撃一覧
	ATKData startATKData_;

	//最大コンボ数
	const int maxATKConBo = 3;

	int ATKConboCount = 1;

	enum class ATKState {
		Extra,//準備
		ATK,	//攻撃実行
		Rigor//硬直
	};
	//攻撃の状態enum
	ATKState atkState_;

	//攻撃時に使うデータまとめ
	struct ATKUpdateData {
		//次の攻撃をするか
		bool nextATK = false;
		bool isPushY = false;
		int count = 0;
	};

	//攻撃の更新データ
	ATKUpdateData updateATKData_;

	//実行する攻撃動作
	ATKData ATKData_;

	bool ATKAnimationSetup_ = false;

	enum NowATK {
		kATK1,
		kATK2,
		kATK3,
		_countATK
	};

	NowATK nowATKState_ = kATK1;

};