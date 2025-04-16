#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"SphereCollider/SphereCollider.h"

//落ちる敵情報
class FallEnemy : public InstancingGameObject {

public://**パブリック変数**//

	//パラメータ構造体
	struct Parameters {
		Vector3 velocity = { 0,-1.0f,0 };	 //移動速度
		Vector3 acceraletion = { 0,-1.0f,0 };//加速度
		Vector3 rotate = { 0,0,0 };			 //回転速度
		float deadSec = 0;					 //死亡カウント
		float repulsion = 1.0f;				 //反発力
		float repulsionHeight = 0.0f;		 //反発する高さ
		bool isDead = false;				 //死亡フラグ
	};




public://**パブリック関数**//

	//コンストラクタ
	FallEnemy(const EulerWorldTransform& world ,const Parameters&param);
	~FallEnemy() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ヒット確認
	/// </summary>
	/// <param name="collider">対象コライダー</param>
	/// <returns></returns>
	bool IsHit(SphereCollider*collider);

	/// <summary>
	/// 死亡フラグチェック
	/// </summary>
	/// <returns></returns>
	bool IsDead()const { return parameters_.isDead; }

private://***プライベート関数**//

	/// <summary>
	/// ヒット時の処理
	/// </summary>
	void OnCollision();

private://**プライベート変数**//

	//コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	//パラメータ
	Parameters parameters_ = {};

};