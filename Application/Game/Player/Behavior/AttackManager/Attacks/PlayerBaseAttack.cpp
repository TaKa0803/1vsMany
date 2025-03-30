#include "PlayerBaseAttack.h"
#include"DeltaTimer/DeltaTimer.h"
#include"Game/Player/Player.h"

//プレイヤーのポインタを初期化
Player* PlayerBaseAttack::player_ = nullptr;

//終了フラグ初期化
bool PlayerBaseAttack::isEnd_ = false;

PlayerBaseAttack::PlayerBaseAttack(){

	//入力マネージャ生成
	inputManager_ = std::make_unique<PlayerInputManager>();

	//共通パラメータをセット
	tree_.SetValue("予備時間", &parameters_.reserveSec);
	tree_.SetValue("攻撃時間", &parameters_.executionSec);
	tree_.SetValue("硬直時間", &parameters_.rigorSec);
	tree_.SetValue("移動速度", &parameters_.speed);
}

void PlayerBaseAttack::Init()
{
	//リクエストをセット
	behaviorRequest_ = Behavior::Reserve;
	
	//フラグを初期化
	isEnd_ = false;
}

void PlayerBaseAttack::Update()
{
	//リクエストが存在する場合
	if (behaviorRequest_) {
		//値を状態に追加
		behavior_ = behaviorRequest_.value();
		//リクエストを初期化
		behaviorRequest_ = std::nullopt;
		//状態を初期化
		(this->*behaviorInit[(int)behavior_])();
	
		//カウント初期化
		currentSec_ = 0;

		//移動量を初期化
		player_->baseParameter_.velocity.SetZero();

		//最大時間を再セット
		SetMaxSec();
	}

	//共通更新
	//時間カウント
	currentSec_ += (float)DeltaTimer::deltaTime_;

	//状態を更新
	(this->*behaviorUpdate[(int)behavior_])();

}

void PlayerBaseAttack::SetInput2Move()
{
	//入力方向を取得して移動を取得
	Vector3 move = player_->SetBody4Input()* parameters_.speed * (float)DeltaTimer::deltaTime_;
	
	//入力が無い場合
	if (move == Vector3(0, 0, 0)) {

		//プレイヤーの向き方向に攻撃
		//オフセット用意
		Vector3 offset = { 0,0,-1 };
		//プレイヤー行列から向きを参照
		offset = TransformNormal(offset, player_->world_.matWorld_);
	
		//上下の向きを無視
		offset.y = 0;

		//正規化
		offset.SetNormalize();

		//速度を加算
		offset *= parameters_.speed * (float)DeltaTimer::deltaTime_;

		//移動量を変更
		move = offset;
	}

	//速度分をセット
	player_->baseParameter_.velocity = move;
}

void PlayerBaseAttack::SetMaxSec()
{
	//各状態の最大時間をセット
	switch (behavior_)
	{
		//攻撃前
	case PlayerBaseAttack::Behavior::Reserve:
		nowMaxSec_ = parameters_.reserveSec;
		break;

		//攻撃中
	case PlayerBaseAttack::Behavior::Execution:
		nowMaxSec_ = parameters_.executionSec;
		break;

		//攻撃後
	case PlayerBaseAttack::Behavior::Rigor:
		nowMaxSec_ = parameters_.rigorSec;
		break;

	default:
		break;
	}
}

//初期化関数をまとめる
void (PlayerBaseAttack::* PlayerBaseAttack::behaviorInit[])() {
	& PlayerBaseAttack::InitReserve,
	& PlayerBaseAttack::InitExecution,
	& PlayerBaseAttack::InitRigor
};

//更新巻数をまとめる
void (PlayerBaseAttack::* PlayerBaseAttack::behaviorUpdate[])() {
	& PlayerBaseAttack::UpdateReserve,
	& PlayerBaseAttack::UpdateExecution,
	& PlayerBaseAttack::UpdateRigor
};