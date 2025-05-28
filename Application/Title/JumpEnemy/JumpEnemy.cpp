#include "JumpEnemy.h"
#include"JumpEnemy.h"
#include "Deltatimer/Deltatimer.h"



JumpEnemy::JumpEnemy(const JumpEnemyData& data)
{

	jumpSpeed_ = &data.speed;
	acceleration_ = &data.acceleration;

	InstancingGameObject::Initialize("TitleEnemy");
}

void JumpEnemy::Update() {

	// ジャンプ中かどうかを確認
	if(isJump_) {

		///ジャンプ処理
		//重力を加算
		velocity_.y += (*acceleration_)*(float)DeltaTimer::deltaTime_;
		//移動量を加算
		world_.translate_ += velocity_ * (float)DeltaTimer::deltaTime_;

		//初期高度より下なら終了
		if (world_.translate_.y < stPosY_) {
			//ジャンプ終了
			isJump_ = false;
			world_.translate_.y = stPosY_;
		}
	}

	InstancingGameObject::Update();
}

void JumpEnemy::Draw() {
	InstancingGameObject::Draw();
}

void JumpEnemy::SetJump()
{
	if (isJump_) return; //すでにジャンプ中なら何もしない
	//フラグをON
	isJump_ = true;
	//jumpの初期位置を保存
	stPosY_ = world_.translate_.y;

	//初期速度を設定
	velocity_ = Vector3{ 0,1,0 }*(*jumpSpeed_);
}
