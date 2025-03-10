#pragma once
#include"Game/Player/Behavior/PlayerBaseBehavior.h"




class PlayerAttack :public PlayerBaseBehavior {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAttack();
	~PlayerAttack() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private://**プライベート関数**//

	/// <summary>
	/// 攻撃前処理
	/// </summary>
	void UpdateExtra();

	/// <summary>
	/// 攻撃中処理
	/// </summary>
	void UpdateAttack();

	/// <summary>
	/// 攻撃後処理
	/// </summary>
	void UpdateRigor();

	//攻撃の状態
	enum AttackType {
		kAttack1,	//攻撃1
		kAttack2,	//攻撃2
		kAttack3,	//攻撃3
		CountATK	//攻撃の数
	};

	/// <summary>
	/// 指定した音の再生
	/// </summary>
	/// <param name="type">音のタイプ</param>
	void PlaySound4Type(AttackType type);

	/// <summary>
	/// 指定した音の停止
	/// </summary>
	/// <param name="type">音のタイプ</param>
	void StopSound4Type(AttackType type);

	/// <summary>
	/// アニメーションを変更
	/// </summary>
	/// <param name="type">アニメーションのタイプ</param>
	void SetAnimation(AttackType type);

private://**プライベート変数

	//現在の攻撃の種類
	AttackType nowAttackType_ = kAttack1;

	//攻撃状態
	enum class AttackBehavior {
		Extra,//準備
		ATK,	//攻撃実行
		Rigor//硬直
	}attackBehavior_;

	//攻撃のデータ構造体
	struct ATKData {
		float extraSec = 1.0f;		//予備動作
		float attackSec = 1.0f;	//攻撃動作
		float rigorSec = 1.0f;		//硬直時間

		float speed = 1.0f;			//移動速度
	};
	//実行する攻撃動作
	ATKData AttackDatas_[CountATK];

	//攻撃時に使うデータまとめ
	struct UpdateData {
		//次の攻撃をするか
		bool nextATK = false;

		//経過時間
		float currentSec = 0.0f;
	}updateData_;

	//状態初期化フラグ
	bool attackBehaviorInit_ = false;
};