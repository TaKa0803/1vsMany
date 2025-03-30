#pragma once
#include"Game/Player/Input/PlayerInputManager.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"
#include<optional>
#include<memory>

//プレイヤー前方宣言
class Player;

//プレイヤーの攻撃基底クラス
class PlayerBaseAttack {

protected://**共通変数**//

	//プレイヤーポインタ
	static Player* player_;

	//終了フラグ
	static bool isEnd_;

public://**パブリック変数**//

	//デバッグツリー
	GvariTree tree_;

	//入力マネージャ
    std::unique_ptr<PlayerInputManager>inputManager_;
	//現在の最大時間
	float nowMaxSec_ = 1.0f;		
	//現在の経過時間
	float currentSec_ = 0.0f;		
public://**パブリック関数**//

	PlayerBaseAttack();
	virtual ~PlayerBaseAttack()=default;

	/// <summary>
	/// 共通初期化
	/// </summary>
	virtual void Init()final;

	/// <summary>
	/// 共通更新
	/// </summary>
	virtual void Update()final;

public://**セッター・ゲッター**//

	/// <summary>
	/// 入力方向に向いて移動
	/// </summary>
	void SetInput2Move();

	/// <summary>
	/// プレイヤーポインタをセット
	/// </summary>
	/// <param name="player"></param>
	static void SetPlayer(Player* player) {
		player_ = player;
	}

	/// <summary>
	/// 終了フラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	static bool& GetAttackData() {
		return isEnd_;
	}



private://**プライベート関数**//

	/// <summary>
	/// 最大時間をセット
	/// </summary>
	void SetMaxSec();


public://**状態関数**//

	//攻撃状態
	enum class Behavior {
		Reserve,	//準備
		Execution,	//実行
		Rigor		//硬直
	};

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = std::nullopt;

	//各状態の初期化
	virtual void InitReserve()=0;
	virtual void InitExecution() = 0;
	virtual void InitRigor() = 0;

	//各状態更新巻数
	virtual void UpdateReserve() = 0;
	virtual void UpdateExecution() = 0;
	virtual void UpdateRigor() = 0;

	//関数テーブル
	static void (PlayerBaseAttack::* behaviorInit[])();
	static void (PlayerBaseAttack::* behaviorUpdate[])();



private://**プライベート変数**//

	//現在の状態
	Behavior behavior_ = Behavior::Reserve;

	//攻撃の共通パラメータ構造体
	struct AttackParameters {
		float reserveSec = 1.0f;	//予備動作
		float executionSec = 1.0f;	//攻撃動作
		float rigorSec = 1.0f;		//硬直時間

		float speed = 1.0f;			//移動速度
	}parameters_;


};