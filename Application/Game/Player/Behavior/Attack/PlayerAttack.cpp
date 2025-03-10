#include "PlayerAttack.h"
#include"Game/Player/Player.h"
#include"DeltaTimer/DeltaTimer.h"
#include<numbers>

PlayerAttack::PlayerAttack()
{

	//デバッグ用にパラメータをツリーにセット
	tree_.name_ = "パンチ攻撃";

	tree_.SetMonitorValue("初期化フラグ", &attackBehaviorInit_);
	tree_.SetMonitorValue("次の攻撃フラグ", &updateData_.nextATK);
	tree_.SetMonitorValue("経過時間", &updateData_.currentSec);

	//攻撃の名前
	std::string treeNames[CountATK] = {
		"攻撃1",
		"攻撃2",
		"攻撃3"
	};

	for (int i = 0; i < CountATK; i++) {
		GvariTree newTree;
		newTree.name_ = treeNames[i];
		newTree.SetValue("準備時間", &AttackDatas_[i].extraSec);
		newTree.SetValue("攻撃時間", &AttackDatas_[i].attackSec);
		newTree.SetValue("硬直時間", &AttackDatas_[i].rigorSec);
		newTree.SetValue("速度", &AttackDatas_[i].speed);
		//作ったツリーを追加
		tree_.SetTreeData(newTree);
	}

}

void PlayerAttack::Init()
{
	//状態リクエストを消す
	behaviorRequest_ = std::nullopt;

	//最初の攻撃の指定
	nowAttackType_ = kAttack1;

	//攻撃の状態を初期化
	attackBehavior_ = AttackBehavior::Extra;

	//攻撃時の入力受け取り変数の初期化
	updateData_ = UpdateData{};

	//初期化済フラグをOFF
	attackBehaviorInit_ = false;

	//現在の移動ベクトルを無くす
	player_->parameter_.velocity.SetZero();

}

void PlayerAttack::Update()
{

	//各状態の初期化が済んでいる場合カウント
	if (attackBehaviorInit_) {
		//時間を加算
		updateData_.currentSec += (float)DeltaTimer::deltaTime_;
	}

	//攻撃入力があれば保存しておく
	if (input_->GetAttackInput()) {
		updateData_.nextATK = true;
	}

	//常にコンボ数をプレイヤーに渡す
	player_->parameter_.comboCount = (int)nowAttackType_;

	//各状態での更新処理
	if (attackBehavior_ == AttackBehavior::Extra) {
		//攻撃前の更新処理
		UpdateExtra();
	}
	else if (attackBehavior_ == AttackBehavior::ATK) {
		//攻撃中の更新処理
		UpdateAttack();
	}
	else if (attackBehavior_ == AttackBehavior::Rigor) {
		//攻撃後の更新処理
		UpdateRigor();
	}

	
}

void PlayerAttack::UpdateExtra()
{
	//初期設定
	if (!attackBehaviorInit_) {
		attackBehaviorInit_ = true;
		SetAnimation(nowAttackType_);
	}
	else {

		//入力方向を向く
		player_->SetBody4Input();

		//条件を満たしたら次の状態へ
		if (updateData_.currentSec >= AttackDatas_[nowAttackType_].extraSec) {
			//次の状態へ
			attackBehavior_ = AttackBehavior::ATK;
			//カウント初期化
			updateData_.currentSec = 0;
			//初期化フラグ初期化
			attackBehaviorInit_ = false;
		}
	}
}

void PlayerAttack::UpdateAttack()
{
	if (!attackBehaviorInit_) {
		attackBehaviorInit_ = true;

		//音の再生
		PlaySound4Type(nowAttackType_);

		//攻撃エフェクト出現
		player_->SpawnImpactEffect();

	}
	else {

		//予備動作中向き変更
		Vector3 move = player_->SetBody4Input();

		//入力がある場合
		if (move != Vector3(0, 0, 0)) {
			//速度加算
			move *= AttackDatas_[nowAttackType_].speed;
			//ベクトルを変更
			player_->parameter_.velocity = move;
		}
		else {
			//プレイヤーの向き方向に攻撃
			//オフセット用意
			Vector3 offset = { 0,0,-1 };
			//プレイヤー行列から向きを参照
			offset = TransformNormal(offset, player_->world_.matWorld_);

			//速度を加算
			offset *= AttackDatas_[nowAttackType_].speed;
			//上下の向きを無視
			offset.y = 0;

			//プレイヤーのベクトルに変更
			player_->parameter_.velocity = offset;
		}

		//条件を満たしたら次の状態へ
		if (updateData_.currentSec >= AttackDatas_[nowAttackType_].attackSec) {

			//状態を変更
			attackBehavior_ = AttackBehavior::Rigor;
			//カウントの初期化
			updateData_.currentSec = 0;
			//状態初期化済フラグをOFF
			attackBehaviorInit_ = false;
		}

	}
}

void PlayerAttack::UpdateRigor()
{
	//初期化がまだの場合初期化
	if (!attackBehaviorInit_) {
		attackBehaviorInit_ = true;
		//移動速度を0に
		player_->parameter_.velocity.SetZero();
	}
	else {

		//条件を満たしたら次の状態へ
		if (updateData_.currentSec >= AttackDatas_[nowAttackType_].rigorSec) {

			//音を停止
			StopSound4Type(nowAttackType_);

			//攻撃入力フラグON&次に攻撃が存在している場合
			if (updateData_.nextATK && nowAttackType_ != kAttack3) {

				//データを初期化
				updateData_ = UpdateData{};
				//初期化済フラグをOFFに
				attackBehaviorInit_ = false;
				//状態を攻撃前に変更
				attackBehavior_ = AttackBehavior::Extra;

				//状態の変更
				//次の状態に変更
				if (nowAttackType_ == kAttack1) {
					nowAttackType_ = kAttack2;
				}
				else if (nowAttackType_ == kAttack2) {
					nowAttackType_ = kAttack3;
				}
			}
			else {
				//移動状態に変更
				behaviorRequest_ = State::Move;
				//カウントを0にセット
				player_->parameter_.comboCount = 0;
			}
		}
	}
}

void PlayerAttack::PlaySound4Type(AttackType type)
{
	//タイプに合わせて再生
	switch (type)
	{
	case PlayerAttack::kAttack1:
		//攻撃1の音再生
		player_->SetPlaySound(PlayerSoundManager::Punch);
		break;

	case PlayerAttack::kAttack2:
		//攻撃2の音再生
		player_->SetPlaySound(PlayerSoundManager::Kick);
		break;

	case PlayerAttack::kAttack3:
		//攻撃3の音再生
		player_->SetPlaySound(PlayerSoundManager::Drill);
		break;

	case PlayerAttack::CountATK:
		break;
	default:
		break;
	}
}

void PlayerAttack::StopSound4Type(AttackType type)
{
	//タイプに合わせて停止
	switch (type)
	{
	case PlayerAttack::kAttack1:
		//攻撃1の音再生
		player_->SetStopSound(PlayerSoundManager::Punch);
		break;

	case PlayerAttack::kAttack2:
		//攻撃2の音再生
		player_->SetStopSound(PlayerSoundManager::Kick);
		break;

	case PlayerAttack::kAttack3:
		//攻撃3の音再生
		player_->SetStopSound(PlayerSoundManager::Drill);
		break;

	case PlayerAttack::CountATK:
		break;
	default:
		break;
	}
}

void PlayerAttack::SetAnimation(AttackType type)
{
	//タイプに合わせて変更
	switch (type)
	{
	case PlayerAttack::kAttack1:
		//攻撃1の音再生
		player_->SetAnimation(PlayerAnimationManager::ATK1);
		break;

	case PlayerAttack::kAttack2:
		//攻撃2の音再生
		player_->SetAnimation(PlayerAnimationManager::ATK2);
		break;

	case PlayerAttack::kAttack3:
		//攻撃3の音再生
		player_->SetAnimation(PlayerAnimationManager::ATK3);
		break;

	case PlayerAttack::CountATK:
		break;
	default:
		break;
	}
}
