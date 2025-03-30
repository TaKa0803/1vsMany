#pragma once
#include"Input/Input.h"
#include"Camera/Camera.h"
#include"Game/Player/Input/PlayerInputManager.h"

#include<optional>

//前方宣言
class Player;

enum class State {
	Move,		//移動
	ATK,		//攻撃
	HITACTION,	//被攻撃時
	SPECIALATK,	//特別攻撃
	NumStates	//状態の数
};

//基本パラメータ
struct PlayerParameter {
	//向き
	Vector3 rotation;
	//速度
	Vector3 velocity;
	//加速度
	Vector3 acceleration;

	//コンボ数
	int comboCount;
};

/// <summary>
/// プレイヤーの状態の基底クラス
/// </summary>
class PlayerBaseBehavior {

protected://**共通変数**//

	//状態リクエスト
	static std::optional<State>behaviorRequest_;

	//プレイヤー
	static Player* player_;

	//カメラのインスタンス
	const Camera* camera_ = nullptr;

public://**パブリック変数**//

	//パラメータ保存ツリー
	GvariTree tree_;

public://**パブリック関数**//

	PlayerBaseBehavior();
	virtual ~PlayerBaseBehavior() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// プレイヤーポインタのセット
	/// </summary>
	/// <param name="player"></param>
	static void SetPlayer(Player* player) { player_ = player; };

	/// <summary>
	/// 状態リクエストを取得
	/// </summary>
	/// <returns>リクエスト変数</returns>
	static const std::optional<State>& GetBehaviorRequest() { return behaviorRequest_; }

	/// <summary>
	/// 状態リクエストを初期化
	/// </summary>
	static void SetBehaviorRequestNull() { behaviorRequest_ = std::nullopt; }
};