#include "PlayerAttack.h"
#include"Game/Player/Player.h"
#include<numbers>

PlayerAttack::PlayerAttack()
{
	//状態リクエストを消す
	behaviorRequest_ = std::nullopt;
}

void PlayerAttack::Init()
{
	//model_->ChangeAnimation(animeName_[ATK1], 5.0f / 60.0f);
	//model_->SetAnimationRoop(false);
	//model_->animationRoopSecond_ = 10.0;

	player_->SetAnimation(PlayerAnimationManager::ATK1);

	nowATKState_ = kATK1;

	atkState_ = ATKState::Extra;

	updateATKData_ = ATKUpdateData{};

	//どちらのボタンが最初かで攻撃ツリー変更

	ATKData_ = startATKData_;

	ATKAnimationSetup_ = false;

	ATKConboCount = 1;
}

void PlayerAttack::Update()
{
#pragma region 実行処理
	if (atkState_ == ATKState::Extra) {

		//初期設定
		if (!ATKAnimationSetup_) {
			ATKAnimationSetup_ = true;
		}
		else {
			//時間の加算
			updateATKData_.count++;
			//条件を満たしたら次の状態へ
			if (updateATKData_.count >= ATKData_.extraTime) {
				//次の状態へ
				atkState_ = ATKState::ATK;
				//カウント初期化
				updateATKData_.count = 0;
				//初期化フラグ初期化
				ATKAnimationSetup_ = false;
			}

		}
	}

	if (atkState_ == ATKState::ATK) {

		if (!ATKAnimationSetup_) {
			ATKAnimationSetup_ = true;

			if (ATKConboCount == 1) {
				//AudioManager::PlaySoundData(punchSound_, 0.05f);
			}
			else if (ATKConboCount == 2) {
				//AudioManager::PlaySoundData(kickSound_, 0.05f);
			}
			else if (ATKConboCount == 3) {
				//AudioManager::PlaySoundData(drilSound_, 0.05f);
			}

			//攻撃エフェクト出現
			player_->SpawnImpactEffect();

		}
		else {

			//予備動作中向き変更
			Vector3 move = input_->GetMoveInput();

			//カメラ方向に向ける
			move = TransformNormal(move, camera_->GetMainCamera().matWorld_);
			//高さを無視
			move.y = 0;

			//入力がある場合
			if (move != Vector3(0, 0, 0)) {
				//向きを入力方向に向ける
				player_->parameter_.rotation.y = GetYRotate({ move.x,move.z }) + ((float)std::numbers::pi);
				//速度加算
				move *= ATKData_.spd;
				//ベクトルを変更
				player_->parameter_.velocity = move;
			}
			else {
				//プレイヤーの向き方向に攻撃

				//オフセット用意
				Vector3 offset = { 0,0,1 };
				//プレイヤー行列から向きを参照
				offset = TransformNormal(offset, player_->world_.matWorld_);
				//player_->parameter_.rotation.y = GetYRotate({ offset.x,offset.z }) + ((float)std::numbers::pi);

				//速度を加算
				offset *= ATKData_.spd;
				//上下の向きを無視
				offset.y = 0;

				//プレイヤーのベクトルに変更
				player_->parameter_.velocity = offset;
			}

			updateATKData_.count++;
			//条件を満たしたら次の状態へ
			if (updateATKData_.count >= ATKData_.AttackTime) {


				atkState_ = ATKState::Rigor;
				updateATKData_.count = 0;
				ATKAnimationSetup_ = false;
			}

		}
	}

	if (atkState_ == ATKState::Rigor) {
		updateATKData_.count++;
		//条件を満たしたら次の状態へ
		if (updateATKData_.count >= ATKData_.RigorTime) {
#pragma region 条件によるシーン転換

			if (ATKConboCount == 1) {
				//AudioManager::StopSound(punchSound_);
			}
			else if (ATKConboCount == 2) {
				//AudioManager::StopSound(kickSound_);
			}
			else if (ATKConboCount == 3) {
				//AudioManager::StopSound(drilSound_);
			}


			//攻撃入力フラグON
			if (updateATKData_.nextATK && nowATKState_ != kATK3 && ATKData_.ATKDerivation.size() != 0) {

				ATKData_ = ATKData_.ATKDerivation[0];

				updateATKData_ = ATKUpdateData{};
				ATKAnimationSetup_ = false;
				atkState_ = ATKState::Extra;

				ATKConboCount++;

				if (nowATKState_ == kATK1) {
					nowATKState_ = kATK2;
					//model_->ChangeAnimation(animeName_[ATK2], 5.0f / 60.0f);
				}
				else if (nowATKState_ == kATK2) {
					nowATKState_ = kATK3;
					//model_->ChangeAnimation(animeName_[ATK3], 5.0f / 60.0f);
				}
			}
			else {
				//移動状態に変更
				behaviorRequest_ = State::Move;
				ATKConboCount = 0;
			}
#pragma endregion

		}
	}
#pragma endregion

#pragma region 実行中のキー入力受付

	if (input_->GetAttackInput()) {
		updateATKData_.nextATK = true;
	}
#pragma endregion
}
