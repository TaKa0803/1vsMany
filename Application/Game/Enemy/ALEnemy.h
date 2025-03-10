#pragma once

#include"SingleGameObjects/GameObject.h"
#include"SphereCollider/SphereCollider.h"
#include<iostream>
#include"Game/items.h"


class Enemy : public InstancingGameObject {

public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(const Vector3& position, const EulerWorldTransform* playerWorld);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	bool Collision(SphereCollider* collider);

	//osimodosi
	Vector3 OshiDashi(SphereCollider* collider);

	//押し戻し処理
	void PushBack(const Vector3& backV);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//死んだか否か
	bool GetDead() { return isDead_; }

	//すでに当たっているか否か
	bool isHit() { return isHit_; }

	void AddTranslate(const Vector3& translate) { world_.translate_ += translate; }

	SphereCollider* GetCollider() { return collider_.get(); }
private:
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

#pragma region 影
	std::unique_ptr<InstancingGameObject>shadow;
#pragma endregion


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