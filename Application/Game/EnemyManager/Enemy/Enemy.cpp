#include"Game/EnemyManager/Enemy/Enemy.h"
#include"DeltaTimer/DeltaTimer.h"
#include"RandomNum/RandomNum.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"

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
	circleShadow = std::make_unique<CircleShadow>(world_);

	//各パラメータを設定
	//ランダム移動速度を生成
	maxSpped_ = RandomNumber::Get(param.minSpeed, param.maxSpeed);
	//最大速度になるまでの時間
	maxSpeedSec_ = param.maxSpeedSec;
	//加速度
	acceraletion_ = param.acceraletion;
	//追跡停止する最小距離
	stopFollowRange_ = param.stopFollowRange;
	//追跡開始する最大距離
	startFollowRange_ = param.startFollowRange;

	//ヒット時の速度
	hitSPD_ = param.hitSpeed;

	//高さの向き
	hitHighVelo_ = param.hitHighVelo;

	//体力
	HP_ = param.hp;

	collider_ = std::make_unique<SphereCollider>();
	collider_->Initialize("ene", world_);
	collider_->SetRadius(1.5f);
	collider_->SetTranslate({ 0,1.5f,0 });
}

//関数ポインタ設定
//初期化関数まとめ
void (Enemy::* Enemy::BehaviorInitialize[])() = {
	&Enemy::StayInitialize,
	&Enemy::FollowInitialize,
	&Enemy::HitInitialize
};

//更新巻数まとめ
void (Enemy::* Enemy::BehaviorUpdate[])() = {
	&Enemy::StayUpdate,
	&Enemy::FollowUpdate,
	&Enemy::HitUpdate
};

void Enemy::Update() {
	//ヒットフラグを無効化
	isHit_ = false;

	//状態の初期化処理
	if (behaviorRequest_) {
		//リクエストの値を設定
		behavior_ = behaviorRequest_.value();
		//リクエスト初期化
		behaviorRequest_ = std::nullopt;

		//実際の初期化処理
		(this->*BehaviorInitialize[(int)behavior_])();
	}

	//状態の更新
	(this->*BehaviorUpdate[(int)behavior_])();

	//加算処理
	world_.translate_ += velocity_ * (float)DeltaTimer::deltaTime_;

	//各更新
	world_.UpdateMatrix();
	collider_->Update();
	circleShadow->Update();
}

void Enemy::Draw() {

	//行列更新
	world_.UpdateMatrix();

	//状態で描画
	if (behavior_ == Stay) {
		//タグに対応したモデルにワールド追加
		IMM_->SetData(waitAnimationTag_, world_);
	}
	else {
		//タグに対応したモデルにワールド追加
		IMM_->SetData(moveAnimationTag_, world_);
	}

	//丸影描画
	circleShadow->Draw();

	//コライダーは当たり判定に違和感があった時だけ描画
	//collider_->Draw();
}


bool Enemy::Collision(SphereCollider* collider) {

	//押し戻しベクトルを作成
	Vector3 backVec;
	//もし当たっていれば処理
	if (collider_->IsCollision(collider, backVec)) {
		//状態リクエストを設定
		behaviorRequest_ = Hit;
		//ＴＲＵＥで返却
		return true;
	}
	//当たらなかかった
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
			acceraletion_.y = 0;
		}
	}

	return backVec;
}



#pragma region 各状態の初期化と更新

void Enemy::StayInitialize()
{
	//移動量を0に変更
	velocity_.SetZero();
}

void Enemy::FollowInitialize()
{
}

void Enemy::HitInitialize()
{
	//自身の座標取得
	Vector3 myPos = world_.GetWorldTranslate();
	//プレイヤー座標取得
	Vector3 playerPos = playerWorld_->GetWorldTranslate();

	//どちらも高さを考慮しない
	myPos.y = 0;
	playerPos.y = 0;

	//向きベクトルを求める
	Vector3 direc = myPos - playerPos;
	//高さを設定
	direc.y = hitHighVelo_;

	//正規化
	direc.SetNormalize();
	direc *= hitSPD_;

	//初速度を設定
	velocity_ = direc;

	//体力を減らす
	HP_--;

	//浮かせる
	world_.translate_.y = 0.01f;
}

void Enemy::StayUpdate()
{
	//プレイヤーまでのベクトル
	Vector3 p_eVelo = playerWorld_->GetWorldTranslate() - world_.GetWorldTranslate();
	//高さを考慮しない
	p_eVelo.y = 0;
	//長さを出す
	float p_eLength = p_eVelo.GetLength();

	//追跡範囲内にプレイヤーがいる時に追従状態に変更
	if (p_eLength > stopFollowRange_ && p_eLength < startFollowRange_) {
		//状態リクエストを設定
		behaviorRequest_ = Follow;
	}
}

void Enemy::FollowUpdate()
{
	//プレイヤー方向のベクトル
	Vector3 p_eVelo = playerWorld_->GetWorldTranslate() - world_.GetWorldTranslate();
	//高さを考慮しない
	p_eVelo.y = 0;

	//長さを出す
	float p_eLength = p_eVelo.GetLength();

	//プレイヤーが追跡範囲内の時
	if (p_eLength > stopFollowRange_ && p_eLength < startFollowRange_) {

		//プレイヤーの方向に移動
		Vector3 moveVelo{};
		//向きベクトルを設定
		moveVelo = p_eVelo;
		//正規化
		moveVelo.SetNormalize();
		//移動速度を加算
		currentSpeedSec_ += (float)DeltaTimer::deltaTime_;

		//割合Tを算出
		float t = currentSpeedSec_ / maxSpeedSec_;
		//割合から現在の速度算出
		float nowSpeed = Lerp(0, maxSpped_, t);
		//速度量を求める
		moveVelo *= nowSpeed;
		//速度に追加
		velocity_ += moveVelo;

		//速度を取得
		float spd = Length(velocity_);
		//最大速度に達していたら移動量もどす
		if (spd > maxSpped_) {
			velocity_.SetNormalize();
			//最大速度に変換
			velocity_ *= maxSpped_;
		}

		//向きの処理
		if (moveVelo != Vector3(0, 0, 0)) {
			world_.rotate_.y = GetYRotate({ moveVelo.x,moveVelo.z })+(float)std::numbers::pi;
		}
	}
	else {
		//範囲外なので待機状態にリクエスト
		behaviorRequest_ = Stay;

	}
}

void Enemy::HitUpdate()
{
	//加速度を加算
	velocity_ += acceraletion_ * (float)DeltaTimer::deltaTime_;

	//回転する
	world_.rotate_.x -= 0.5f;

	//高さが規定値以下なら処理
	if (world_.translate_.y <= tHeight) {

		//HPがあるなら復帰
		if (HP_ > 0) {
			//高さを合わせる
			world_.translate_.y = tHeight;
			//角度を戻す
			world_.rotate_.x = 0;
			//状態リクエストを設定
			behaviorRequest_ = Stay;
			//移動量を初期化
			velocity_.SetZero();
		}
		else {
			//死亡フラグを有効
			isDead_ = true;
		}
	}
}
#pragma endregion
