#pragma once
#include"Game/EnemyManager/ParameterManager/EnemyParameterManager.h"
#include"SphereCollider/SphereCollider.h"
#include"Game/CircleShadow/CircleShadow.h"
#include<iostream>

//敵キャラクラス
class Enemy : public InstancingGameObject {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">出現位置</param>
	/// <param name="playerWorld">プレイヤーワールドポインタ</param>
	/// <param name="param">パラメータ</param>
	Enemy(const Vector3 position, const EulerWorldTransform* playerWorld, const EnemyParameters& param);
	~Enemy() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ヒット確認と押し戻し処理
	/// </summary>
	/// <param name="collider">判定するコライダー</param>
	/// <returns></returns>
	bool Collision(SphereCollider* collider);

	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="collider">参照元コライダー</param>
	/// <returns>押し戻しベクトル</returns>
	Vector3 OshiDashi(SphereCollider* collider);

	/// <summary>
	/// 死亡チェック
	/// </summary>
	/// <returns></returns>
	bool GetDead() const { return isDead_; }

	/// <summary>
	/// 既にヒット済かチェック
	/// </summary>
	/// <returns></returns>
	bool isHit()const { return isHit_; }

	/// <summary>
	/// 座標を加算
	/// </summary>
	/// <param name="translate"></param>
	void AddTranslate(const Vector3& translate) { world_.translate_ += translate; }

	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns>コライダーポインタ返却</returns>
	SphereCollider* GetCollider() { return collider_.get(); }

private://**状態に関するプライベート関数**//

	//各状態初期化
	void StayInitialize();
	void FollowInitialize();
	void HitInitialize();

	//各状態更新
	void StayUpdate();
	void FollowUpdate();
	void HitUpdate();

	//状態ごとの初期化テーブル
	static void (Enemy::* BehaviorInitialize[])();
	//状態ごとの更新テーブル
	static void (Enemy::* BehaviorUpdate[])();

private://**状態に関するプライベート関数**//

	//行動状態
	enum State {
		Stay,	//通常
		Follow, //追従
		Hit		//攻撃ヒット
	}behavior_ = Stay;

	//状態リクエスト
	std::optional<State>behaviorRequest_ = std::nullopt;

private://**プライベート変数**//

	//プレイヤーのワールド
	const EulerWorldTransform* playerWorld_;

	//当たり判定コライダー
	std::unique_ptr<SphereCollider>collider_;

	//丸影
	std::unique_ptr<CircleShadow>circleShadow;

	//待機状態animationモデルタグ
	std::string waitAnimationTag_ = "EnemyWait";
	//移動状態アニメーションモデルタグ
	std::string moveAnimationTag_ = "EnemyMove";

	//移動速度
	Vector3 velocity_{};

	//死亡フラグ
	bool isDead_ = false;

	//規定高度
	float tHeight = 0;

	//ヒットフラグ
	bool isHit_ = false;

	//現在の速度
	float currentSpeedSec_ = 0;

private://**パラメータ変数**//

	//最大HP
	int HP_ = 1;

	//ヒット時の上向きの初速度
	float hitHighVelo_ = 1.5f;

	//最大速度になるまでの時間
	float maxSpeedSec_ = 6.0f;

	//移動速度
	float maxSpped_ = 0.5f;

	//加速度
	Vector3 acceraletion_ = { 0,-0.1f,0 };

	//動かなくなる距離
	float stopFollowRange_ = 10.0f;

	//探知距離
	float startFollowRange_ = 30.0f;

	//吹っ飛び量
	float hitSPD_ = 0.5f;

};