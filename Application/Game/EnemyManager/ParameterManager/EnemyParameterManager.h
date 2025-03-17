#pragma once
#include"Vector3.h"
#include"Game/BaseManager/BaseManager.h"
#include<string>

//敵のパラメータ群
struct EnemyParameters {
	//最大速度になるまでの時間
	float maxSpeedSec = 1.0f;

	//個体最大速度
	float maxSpeed = 0.5f;
	//個体最低速度
	float minSpeed = 0.3f;

	//落下速度
	float fallSpeed = 0.1f;

	//接近停止距離
	float stopFollowRange = 10.0f;

	//追従開始距離
	float startFollowRange = 30.0f;

	//加速度
	Vector3 acceraletion = {0,-0.1f,0};

	//ヒット時の速度
	float hitSpeed = 1.0f;

	//上向きの吹っ飛び量
	float hitHighVelo = 1.5f;

	//体力
	int hp = 1;
};

//敵のパラメータ管理クラス
class EnemyParameterManager :public BaseManager{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyParameterManager();
	~EnemyParameterManager()=default;

	/// <summary>
	/// アニメーション関係のパラメータの反映
	/// </summary>
	void SetAnimationParameter() const;

	/// <summary>
	/// パラメータの取得
	/// </summary>
	/// <returns></returns>
	const EnemyParameters& GetParameters() { return parameters_; };

private://**プライベート変数**//

	//敵のパラメータデータ群
	EnemyParameters parameters_;

	//Idleアニメーション再生倍率
	float stayAnimationMultiplySpeed = 1.0f;
	//移動アニメーション再生倍率
	float moveAnimationMultiplySpeed = 5.0f;

	//移動用インスタンシングモデルtag
	std::string stayAnimationModelTag_ = "PlayerM3";
	//停止用インスタンシングモデルtag
	std::string moveAnimationModelTag_ = "PlayerM4";

};