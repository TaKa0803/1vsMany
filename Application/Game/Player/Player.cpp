#include"Player.h"

#include"AudioManager/AudioManager.h"
#include"TextureManager/TextureManager.h"

#include"Game/Player/Behavior/Move/PlayerMove.h"

#include<imgui.h>
#include<json.hpp>
#include<cassert>
#include<fstream>
#include<numbers>

//ATKData LoadATKData(nlohmann::json::iterator& itGroup) {
//	ATKData data;
//
//	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
//
//		//アイテム名
//		const std::string& itemName = itItem.key();
//
//		//各データ
//		if (itemName == "data") {
//			data.extraTime = (float)itItem->at(0);
//			data.AttackTime = (float)itItem->at(1);
//			data.RigorTime = (float)itItem->at(2);
//			data.isYATK = (int)itItem->at(3);
//			data.spd = (float)itItem->at(4);
//		}
//		else if (itemName == "ATKDerivation") {
//			//皇族データ保存
//			data.ATKDerivation.push_back(LoadATKData(itItem));
//		}
//	}
//
//	return data;
//}

//void Player::LoadATKDatas() {
//
//	//読み込み用ファイルストリーム
//	std::ifstream ifs;
//	//ファイルを読み込み用に開く
//	ifs.open(atkDataPass_);
//
//	//読み込み失敗
//	if (ifs.fail()) {
//		assert(false);
//		return;
//	}
//
//	nlohmann::json root;
//	//json文字列からjsonのデータ構造に展開
//	ifs >> root;
//	//移したのでファイルを閉じる
//	ifs.close();
//
//
//
//
//	int Index = 0;
//	//攻撃グループ複数読み込み
//
//		//グループ検索
//	nlohmann::json::iterator itGroup = root.find(groupName_);
//
//	//未登録チェック
//	assert(itGroup != root.end());
//
//
//	startATKData_ = LoadATKData(itGroup);
//
//
//
//	Index++;
//
//}

Player::Player() {
	//一回しかしない初期化情報

	//入力インスタンス生成
	input_ = Input::GetInstance();
	input_->SetDeadLine(0.3f);



	//コライダー生成
	collider_ = std::make_unique<SphereCollider>();
	collider_->Initialize("player", world_);
	collider_->SetRadius(1.5f);
	collider_->SetTranslate({ 0,1.4f,0 });

	//攻撃データの初期化
	//LoadATKDatas();

	GameObject::Initialize("Player");
	model_->SetAnimationActive(true);
	//model_->SetAnimeSecond(10);
	
	//パラメータを設定
	PlayerBaseBehavior::SetPlayer(this);

	//各状態の生成
	behaviors_.resize((size_t)State::NumStates);
	behaviors_[(size_t)State::Move] = std::make_unique<PlayerMove>();


	//インパクトエフェクト生成
	impactE_ = std::make_unique<EffectImpact>();

	//影オブジェクト生成
	shadow = std::make_unique<InstancingGameObject>();

	//移動エフェクト生成
	effectMove_ = std::make_unique<EffectMove>();

	//UIクラス生成
	//ui_ = std::make_unique<PlayerUI>(GetConboCount());

	//アニメーションマネージャの生成
	animationManager_ = std::make_unique<PlayerAnimationManager>(model_.get());

	//サウンドマネージャの生成
	soundManager_ = std::make_unique<PlayerSoundManager>();	
}


void Player::Initialize() {
	//中身データ初期化
	world_.Initialize();
	world_.translate_.z = 2;
	world_.UpdateMatrix();

	//model_->ChangeAnimation(animeName_[WALK], 0);

	effectMove_->Initialize({ 1,1,1,1 });

	ui_->Init();

	moveState_ = NoneS;

	//ATKConboCount = 0;
	//ATKAnimationSetup_ = false;

	shadow->Initialize("DZone");
	shadow->SetParent(&world_);
	shadow->SetColor({ 0,0,0,1 });
	shadow->world_.translate_={ 0,0.01f,0 };
	shadow->SetScale(1.5f);

	impactE_->Initialize();
	
	model_->animationRoopSecond_ = 5.0f;

	collider_->Update();
}

void Player::GameUpdate() {

	if (PlayerBaseBehavior::GetBehaviorRequest()) {
		behaviorRequest_ = PlayerBaseBehavior::GetBehaviorRequest();
	}

	//状態の初期化処理
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;

		//実際の初期化処理
		(this->*BehaviorInitialize[(int)behavior_])();
	}

	//移動エフェクト更新
	effectMove_->Update();

	world_.translate_+=parameter_.velocity;

	//落下の処理
	addFallSpd_ -= fallSpd_;
	world_.translate_.y += addFallSpd_;
	if (world_.translate_.y < 0) {
		world_.translate_.y = 0;
		addFallSpd_ = 0;
	}

	//状態の更新
	(this->*BehaviorUpdate[(int)behavior_])();

}

void Player::ObjectUpdate()
{
	//更新
	world_.UpdateMatrix();
	collider_->Update();
	shadow->Update();
	impactE_->Update();
	ui_->Update();
}

void (Player::* Player::BehaviorInitialize[])() = {
	&Player::InitializeMove,		//移動
	&Player::InitializeATK,		//攻撃
	&Player::InitializeHitAction,	//被攻撃
	&Player::InitializeSpecialATK	//特殊攻撃
};

void (Player::* Player::BehaviorUpdate[])() = {
	&Player::UpdateMove,			//移動
	&Player::UpdateATK,			//攻撃
	&Player::UpdateHitAction,		//被攻撃
	&Player::InitializeSpecialATK	//特殊攻撃
};

void Player::Draw() {

	//各モデル描画

	GameObject::Draw();

	shadow->Draw();

	impactE_->Draw();

	//collider_->Draw();

}

void Player::DrawParticle()
{
	effectMove_->Draw();
}

void Player::DrawUI()
{
	//UIの描画
	ui_->Draw();
}

void Player::DebugWindow(const char* name) {

	name;
	//float cScale = collider_->GetRadius();

	//ImGui::Begin(name);
	//ImGui::DragFloat("spd", &spd_, 0.01f);
	//ImGui::DragFloat("fall spd", &fallSpd_, 0.01f);

	////world_.DrawDebug(name);
	////collider_->Debug(name);
	////model_->DebugParameter(name);

	//ImGui::DragFloat("collider scale", &cScale, 0.1f, 1, 10);

	////model_->DebugParameter(name);
	//ImGui::End();

	//collider_->SetRadius(cScale);
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

void Player::Move() {

	ModelRoop(parameter_.velocity);
}

void Player::ModelRoop(const Vector3& velo)
{
	if (velo.x == 0 && velo.y == 0 && velo.z == 0) {
		//model_->ChangeAnimation(animeName_[WAIT], 15.0f / 60.0f);
		//model_->animationRoopSecond_ = 1.0f;
	}
	else {
		//model_->ChangeAnimation(animeName_[WALK], 30.0f/60.0f);
		//model_->animationRoopSecond_ = 10.0f;
	}

}







#pragma region 各状態初期化処理


void Player::InitializeMove() {

	//model_->ChangeAnimation(3, 0);
	model_->SetAnimationRoop(true);
	model_->animationRoopSecond_ = 5.0;

	moveState_ = StopS;
}

void Player::InitializeATK() {



}

void Player::InitializeHitAction() {

}

void Player::InitializeSpecialATK() {
}

void Player::UpdateSpecialATK() {
}

#pragma endregion

#pragma region 各状態更新処理

void Player::UpdateMove() {

	behaviors_[(size_t)State::Move]->Update();

}

void Player::UpdateATK() {







}

void Player::UpdateHitAction() {

}

#pragma endregion