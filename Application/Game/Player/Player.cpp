#include"Player.h"

#include"Game/Player/Behavior/Move/PlayerMove.h"
#include"Game/Player/Behavior/Attack/PlayerAttack.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#include<numbers>


Player::Player() {

	//入力管理クラス生成
	input_ = std::make_unique<PlayerInputManager>();

	//カメラのインスタンス取得
	camera_ = Camera::GetInstance();

	//コライダー生成
	collider_ = std::make_unique<SphereCollider>();
	collider_->Initialize("player", world_);
	collider_->SetRadius(1.5f);
	collider_->SetTranslate({ 0,1.4f,0 });

	//円影の生成
	circleShadow_ = std::make_unique<CircleShadow>(world_);

	//オブジェクト生成
	GameObject::Initialize("Player");
	
	//パラメータを設定
	PlayerBaseBehavior::SetPlayer(this);

	//各状態の生成
	behaviors_.resize((size_t)State::NumStates);
	behaviors_[(size_t)State::Move] = std::make_unique<PlayerMove>();
	behaviors_[(size_t)State::ATK] = std::make_unique<PlayerAttack>();

	//インパクトエフェクト生成
	impactE_ = std::make_unique<EffectImpact>();

	//移動エフェクト生成
	effectMove_ = std::make_unique<EffectMove>();

	//UIクラス生成
	ui_ = std::make_unique<PlayerUI>(parameter_.comboCount);

	//アニメーションマネージャの生成
	animationManager_ = std::make_unique<PlayerAnimationManager>(model_.get());

	//サウンドマネージャの生成
	soundManager_ = std::make_unique<PlayerSoundManager>();	

	//プレイヤーのデバッグ用パラメータ設定
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GlobalVariableGroup>("player");
	gvg->SetTreeData(model_->SetDebugParam());
	gvg->SetTreeData(animationManager_->GetTree());

	gvg->SetTreeData(behaviors_[(size_t)State::Move]->tree_);
	gvg->SetTreeData(behaviors_[(size_t)State::ATK]->tree_);

}


void Player::Initialize() {
	//中身データ初期化
	world_.Initialize();
	world_.translate_.z = 2;

	//移動エフェクトの初期化
	effectMove_->Initialize({ 1,1,1,1 });

	//UIの初期化
	ui_->Init();

	//衝撃エフェクト初期化
	impactE_->Initialize();
}

void Player::GameUpdate() {

	//状態の初期化処理
	if (PlayerBaseBehavior::GetBehaviorRequest()) {
		//値を現在の状態に渡す
		behavior_ = PlayerBaseBehavior::GetBehaviorRequest().value();

		//状態の初期化
		behaviors_[(int)behavior_]->Init();
	}

	//状態の更新
	behaviors_[(int)behavior_]->Update();

	//移動エフェクト更新
	effectMove_->Update();

	//移動量加算
	world_.translate_+=parameter_.velocity;
	//変更した回転量を渡す
	world_.rotate_ = parameter_.rotation;

	//落下の処理
	addFallSpd_ -= fallSpd_;
	world_.translate_.y += addFallSpd_;
	if (world_.translate_.y < 0) {
		world_.translate_.y = 0;
		addFallSpd_ = 0;
	}


}

void Player::ObjectUpdate()
{
	//更新
	world_.UpdateMatrix();
	circleShadow_->Update();
	collider_->Update();
	
	impactE_->Update();
	ui_->Update();
}



void Player::Draw() {

	//円影描画
	circleShadow_->Draw();

	//各モデル描画
	GameObject::Draw();

	//衝撃演出描画
	impactE_->Draw();

	//コライダー描画
	collider_->Draw();

}

void Player::DrawParticle()
{
	//移動エフェクト描画
	effectMove_->Draw();
}

void Player::DrawUI()
{
	//UIの描画
	ui_->Draw();
}

void Player::OnCollisionBack(const Vector3& backV)
{
	world_.translate_ += backV;
	world_.UpdateMatrix();
	collider_->Update();

	//高さに関する処理が行われた場合落下速度を初期化
	if (backV.y != 0) {
		addFallSpd_ = 0;
	}
}


void Player::SpawnMoveEffect()
{
	//移動エフェクト生成
	effectMove_->SpawnE(world_.GetWorldTranslate());
}

Vector3 Player::SetBody4Input()
{
	//入力を取得
	Vector3 velocity = input_->GetMoveInput();

	//カメラ方向に向ける
	velocity = TransformNormal(velocity, camera_->GetMainCamera().matWorld_);
	//ｙの量を無視する
	velocity.y = 0.0f;

	//正規化
	velocity.SetNormalize();

	//入力がある場合
	if (velocity != Vector3(0, 0, 0)) {
		//向きを指定
		parameter_.rotation.y = GetYRotate({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
	}

	//向きベクトルを返却
	return velocity;
}
