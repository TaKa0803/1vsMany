#pragma once
#include"Vector3.h"
#include<string>

//敵のパラメータ群
struct EnemyParameters {
	//最大速度になるまでの時間
	float maxSpeedSec = 1.0f;

	//個体最大速度
	float maxSpeed_ = 0.5f;
	//個体最低速度
	float minSpeed_ = 0.3f;

	//落下速度
	float fallSpeed_ = 0.1f;

	//接近停止距離
	float stopFollowRange = 10.0f;

	//追従開始距離
	float startFollowRange = 30.0f;

	//加速度
	Vector3 acceraletion = {0,-0.1f,0};

	//吹っ飛び量
	float hitSpeed_ = 0.5f;

	//体力
	int hp = 1;
};

//敵のステータスパラメータ管理クラス
class EnemyStatusManager {

public://**パブリック関数**//

	EnemyStatusManager();
	~EnemyStatusManager()=default;

	EnemyParameters GetParameters() { return parameters_; };

private://**プライベート変数**//

	EnemyParameters parameters_;

	//移動用インスタンシングモデルtag
	std::string a3tag_ = "PlayerM3";
	//停止用インスタンシングモデルtag
	std::string a4tag_ = "PlayerM4";

};