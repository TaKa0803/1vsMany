#include"Game/EnemyManager/Enemy/Enemy.h"

#include"RandomNum/RandomNum.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"Game/BrokenBody/BrokenBody.h"

#include<numbers>



Enemy::Enemy(const Vector3 position, const EulerWorldTransform* playerWorld, const EnemyParameters& param)
{
	//インスタンスモデルマネージャ取得
	IMM_ = InstancingModelManager::GetInstance();

	//座標の設定
	world_.translate_ = position;
	//最低高度を設定
	tHeight = world_.translate_.y;
	//プレイヤーのワールドを取得
	playerWorld_ = playerWorld;

	//出現時ランダムな方向を向く
	world_.rotate_.y = RandomNumber::Get(0, 3.14f);

	//丸影生成
	shadow = std::make_unique<CircleShadow>(world_);

	//各パラメータを設定
	//ランダム移動速度を生成
	moveSpped_ = RandomNumber::Get(param.minSpeed, param.maxSpeed);
	//最大速度になるまでの時間
	maxSpeedSec_ = param.maxSpeedSec;
	//落下速度
	fallspd_ = param.fallSpeed;
	//追跡停止する最小距離
	stopFollowRange_ = param.stopFollowRange;
	//追跡開始する最大距離
	startFollowRange_ = param.startFollowRange;

	collider_ = std::make_unique<SphereCollider>();
	collider_->Initialize("ene", world_);
	collider_->SetRadius(1.5f);
	collider_->SetTranslate({ 0,1.5f,0 });
}


void (Enemy::* Enemy::BehaviorInitialize[])() = {
	&Enemy::StayInitialize,
	&Enemy::FollowInitialize,
	&Enemy::HitInitialize
};

void (Enemy::* Enemy::BehaviorUpdate[])() = {
	&Enemy::StayUpdate,
	&Enemy::FollowUpdate,
	&Enemy::HitUpdate
};

void Enemy::Update() {

	isHit_ = false;

	FallUpdate();

	//状態の初期化処理
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;

		//実際の初期化処理
		(this->*BehaviorInitialize[(int)behavior_])();
	}

	//状態の更新
	(this->*BehaviorUpdate[(int)behavior_])();

	//各更新
	world_.UpdateMatrix();
	collider_->Update();
	shadow->Update();
}



bool Enemy::Collision(SphereCollider* collider) {

	Vector3 backVec;
	if (collider_->IsCollision(collider, backVec)) {

		behavior_ = Hit;

		velocity_ = hitVelo;

		Vector3 myPos = world_.GetWorldTranslate();
		Vector3 pPos = playerWorld_->GetWorldTranslate();

		myPos.y = 0;
		pPos.y = 0;

		Vector3 direc = myPos - pPos;
		direc.SetNormalize();
		direc *= hitSPD_;

		velocity_.x = direc.x;
		velocity_.z = direc.z;

		HP_--;

		behaviorRequest_ = Hit;

		return true;
	}

	return false;
}

Vector3 Enemy::OshiDashi(SphereCollider* collider)
{
	isHit_ = true;

	Vector3 backVec;
	if (collider_->IsCollision(collider, backVec)) {
		backVec.y = 0;
		backVec /= 2;
		world_.translate_ += backVec;

		if (backVec.y != 0) {
			addFallspd_ = 0;
		}
	}

	return backVec;
}

void Enemy::PushBack(const Vector3& backV)
{
	world_.translate_ += backV;
	world_.UpdateMatrix();

	if (backV.y != 0) {
		addFallspd_ = 0;
	}
}

void Enemy::Draw() {

	world_.UpdateMatrix();

	if (behaviorRequest_ == Stay || behavior_ == Stay) {
		//タグに対応したモデルにワールド追加
		IMM_->SetData(a3tag_, world_);
	}
	else{
		//タグに対応したモデルにワールド追加
		IMM_->SetData(a4tag_, world_);
	}
	shadow->Draw();
	//collider_->Draw();
}

#pragma region 各状態の初期化と更新

void Enemy::StayInitialize()
{
	velocity_.SetZero();
	animeNum_ = 3;
}

void Enemy::FollowInitialize()
{
	animeNum_ = 4;

}

void Enemy::HitInitialize()
{
}

void Enemy::StayUpdate()
{
	//pk
	Vector3 p_eVelo = playerWorld_->GetWorldTranslate() - world_.GetWorldTranslate();
	//高さを考慮しない
	p_eVelo.y = 0;

	float p_eLength = p_eVelo.GetLength();

	//プレイヤーが追従範囲内の時
	if (p_eLength > stopFollowRange_ && p_eLength < startFollowRange_) {
		behaviorRequest_ = Follow;

	}
}

void Enemy::FollowUpdate()
{
	//プレイヤー方向の向き
	Vector3 p_eVelo = playerWorld_->GetWorldTranslate() - world_.GetWorldTranslate();
	//高さを考慮しない
	p_eVelo.y = 0;

	float p_eLength = p_eVelo.GetLength();

	//プレイヤーが追従範囲内の時
	if (p_eLength > stopFollowRange_ && p_eLength < startFollowRange_) {

		//プレイヤーの方向に移動
		Vector3 moveVelo{};
		moveVelo = p_eVelo;
		//ノーマライズ
		moveVelo.SetNormalize();
		//移動領分書ける
		moveVelo *= moveSpped_ / maxSpeedSec_;


		//速度に追加
		velocity_ += moveVelo;
		//速度ベクトルの量を取得
		float veloSPD = velocity_.GetLength();
		//プレイヤーへの向きベクトルに乗算
		velocity_ = p_eVelo.SetNormalize() * veloSPD;

		
		//最大速度に達していたら移動量もどす
		float spd = Length(velocity_);
		if (spd > moveSpped_) {
			velocity_.SetNormalize();
			velocity_ *= moveSpped_;
		}

		//加算処理
		world_.translate_ += velocity_;

		//向きの処理
		if (moveVelo != Vector3(0, 0, 0)) {
			world_.rotate_.y = GetYRotate({ moveVelo.x,moveVelo.z })+(float)std::numbers::pi;
		}
	}
	else {
		behaviorRequest_ = Stay;

	}
}

void Enemy::HitUpdate()
{
	velocity_ += acce;

	world_.translate_ += velocity_;

	world_.rotate_.x -= 0.5f;

	//高さがゼロ以下
	if (world_.translate_.y <= tHeight) {

		//HPがあるなら復帰
		if (HP_ > 0) {
			world_.translate_.y = tHeight;
			world_.rotate_.x = 0;
			behavior_ = Stay;
			velocity_.SetZero();

		}
		else {
			//死亡処理
			isDead_ = true;
		}

	}
}
#pragma endregion


void Enemy::FallUpdate()
{
	addFallspd_ -= fallspd_;
	world_.translate_.y += addFallspd_;

	if (world_.translate_.y < 0) {
		world_.translate_.y = 0;
		addFallspd_ = 0;
	}
}
