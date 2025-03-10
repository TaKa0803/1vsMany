#include "PlayerMove.h"
#include"Game/Player/Player.h"
#include<numbers>

void PlayerMove::Init()
{
	//状態リクエストを消す
	behaviorRequest_ = std::nullopt;
}

void PlayerMove::Update()
{
	//移動処理
	Move();

	//アニメーション変更
	ChangeAnimation();

	//遷移処理
	SceneChange();
}

void PlayerMove::Move()
{
	//移動取得
	Vector3 move = input_->GetMoveInput();

	//移動速度量加算
	move *= spd_;

	//カメラ方向に向ける
	move = TransformNormal(move, camera_->GetMainCamera().matWorld_);
	//ｙの量を無視する
	move.y = 0.0f;

	if (move != Vector3(0, 0, 0)) {
		//向きを指定
		player_->parameter_.rotation.y = GetYRotate({ move.x,move.z }) + ((float)std::numbers::pi);
	}
	//加算
	player_->parameter_.velocity = move;
}

void PlayerMove::SceneChange()
{
	//攻撃入力で状態を変更
	if (input_->GetAttackInput()) {
		behaviorRequest_ = State::ATK;
	}
}

void PlayerMove::ChangeAnimation()
{
	//移動していない場合
	if (player_->parameter_.velocity == Vector3{ 0,0,0 }) {
		//停止時のアニメーションに変更
		player_->SetAnimation(PlayerAnimationManager::WAIT);
	}
	else {
		//移動時のアニメーションに変更
		player_->SetAnimation(PlayerAnimationManager::WALK);
	}
}
