#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

struct JumpEnemyData {
	//jump初速度
	float speed;
	//重力加速度
	float acceleration;
};

//画面内でジャンプする敵
class JumpEnemy: public InstancingGameObject{
public:

	JumpEnemy(const JumpEnemyData&data);
	~JumpEnemy()=default;

	//更新
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ワールドデータ取得
	/// </summary>
	/// <returns></returns>
	EulerWorldTransform& GetWorldTransform(){return world_;}

	/// <summary>
	/// jump処理を設定
	/// </summary>
	void SetJump();

private://**プライベート変数**//

	//ジャンプ中かどうか
	bool isJump_ = false;

	//初期位置
	float stPosY_ = 0;

	//移動量
	Vector3 velocity_ = {};

private://**パラメータ変数**//

	//ジャンプ初速度
	const float* jumpSpeed_;

	//重力加速度
	const float* acceleration_;

};