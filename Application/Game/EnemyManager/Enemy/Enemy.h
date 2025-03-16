#pragma once

#include"SingleGameObjects/GameObject.h"
#include"SphereCollider/SphereCollider.h"
#include"Game/CircleShadow/CircleShadow.h"
#include<iostream>

//敵キャラクラス
class Enemy : public InstancingGameObject {

public://**パブリック関数**//

	Enemy();
	~Enemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const EulerWorldTransform* playerWorld);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

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
	/// <returns>おし戻しベクトル</returns>
	Vector3 OshiDashi(SphereCollider* collider);

	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="backV"></param>
	void PushBack(const Vector3& backV);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死亡チェック
	/// </summary>
	/// <returns></returns>
	bool GetDead() { return isDead_; }

	/// <summary>
	/// 既にヒット済かチェック
	/// </summary>
	/// <returns></returns>
	bool isHit() { return isHit_; }

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

	void StayInitialize();
	void FollowInitialize();
	void HitInitialize();

	void StayUpdate();
	void FollowUpdate();
	void HitUpdate();

	//状態ごとの初期化テーブル
	static void (Enemy::* BehaviorInitialize[])();
	//状態ごとの更新テーブル
	static void (Enemy::* BehaviorUpdate[])();

	//落下処理
	void FallUpdate();

private:
	//プレイヤーのワールド
	const EulerWorldTransform* playerWorld_;

	std::unique_ptr<SphereCollider>collider_;

	//丸影
	std::unique_ptr<CircleShadow>shadow;

	std::string a3tag_ = "PlayerM3";
	std::string a4tag_ = "PlayerM4";

	int animeNum_ = 3;

	//移動速度
	Vector3 velocity_{};

	//最大速度になるまでのF数
	float maxSPDFrame = 60.0f;

	//個体差用
	const float maxSPD_ = 0.5f;
	const float minSPD_ = 0.3f;

	//移動速度
	float moveSPD_ = 0.5f;

	//落下速度
	float fallspd_ = 0.1f;
	float addFallspd_ = 0;

	//ugokanakunarukyori
	float stopRange_ = 10.0f;

	//探知距離
	float serchRange_ = 30.0f;

	//行動状態
	enum State {
		Stay,	//通常
		Follow, //追従
		Hit		//攻撃ヒット
	};

	State behavior_ = Stay;
	//状態リクエスト
	std::optional<State>behaviorRequest_ = std::nullopt;

	const Vector3 hitVelo = { 0,1.5f,0 };
	//加速度
	Vector3 acce = { 0,-0.1f,0 };

	//吹っ飛び量
	float hitSPD_ = 0.5f;


	bool isDead_ = false;

	int HP_ = 1;

	float tHeight = 0;

	//破壊音
	int breakSound_;

	bool isHit_ =false;
};