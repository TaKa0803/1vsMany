#include "FallEnemy.h"
#include"DeltaTimer/DeltaTimer.h"



FallEnemy::FallEnemy(const EulerWorldTransform& world, const Parameters& param)
{
	//値の設定
	world_ = world;
	parameters_ = param;
}

void FallEnemy::Update()
{
	//死亡時間カウント
	parameters_.deadSec -= (float)DeltaTimer::deltaTime_;

	//時間が規定を超えたら
	if (parameters_.deadSec <= 0) {
		//死亡フラグを立てる
		parameters_.isDead = true;
	}

	//速度を更新
	parameters_.velocity += parameters_.acceraletion * (float)DeltaTimer::deltaTime_;

	//位置を更新
	world_.translate_ += parameters_.velocity*(float)DeltaTimer::deltaTime_;
	//回転を更新
	world_.rotate_ += parameters_.rotate;

	//0以下で反発
	if (world_.translate_.y <= parameters_.repulsionHeight) {
		//Y座標を0にする
		world_.translate_.y = parameters_.repulsionHeight;
		//Y方向の速度を反発させる
		parameters_.velocity.y *= -parameters_.repulsion;
	}

	//行列を更新
	world_.UpdateMatrix();
}

bool FallEnemy::IsHit(SphereCollider* collider)
{
	collider;
	return false;
}

void FallEnemy::OnCollision()
{
}

