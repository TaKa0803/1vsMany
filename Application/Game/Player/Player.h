#pragma once
#include"SingleGameObjects/GameObject.h"
#include"Input/Input.h"
#include"Camera/Camera.h"
#include"SphereCollider/SphereCollider.h"
#include"Game/Effect/Impact/Impact.h"
#include"Game/items.h"
#include"Game/Effect/EffectMove/EffectMove.h"
#include"Game/Player/UI/PlayerUI.h"
#include"Game/Player/Behavior/PlayerBaseBehavior.h"

#include"Game/Player/Input/PlayerInputManager.h"
#include"Game/Player/Sound/PlayerSoundManager.h"
#include"Game/Player/Animation/PlayerAnimationManager.h"
#include"Game/CircleShadow/CircleShadow.h"
#include<vector>

//プレイヤークラス
class Player :public GameObject {

public://**パブリック変数**//

	//基本パラメータ
	PlayerParameter parameter_;

public://**パブリック変数**//

	Player();
	~Player() = default;

	void Initialize();

	/// <summary>
	/// ゲーム関係の更新
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// モデルの更新
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void DrawParticle();

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	//押し戻し
	void OnCollisionBack(const Vector3& backV);

	//コライダーを取得
	SphereCollider* GetCollider() { return collider_.get(); }

	//攻撃状態か取得
	bool IsPlayerATK() {
		if (behavior_ == State::ATK) { return true; }
		return false;
	}

	/// <summary>
	/// 移動エフェクト出現
	/// </summary>
	void SpawnMoveEffect();

	/// <summary>
	/// 攻撃エフェクト出現
	/// </summary>
	void SpawnImpactEffect() { impactE_->Spawn(world_); };

public://**セッター**//

	/// <summary>
	/// アニメーションのセット
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	void SetAnimation(PlayerAnimationManager::Animation type) { animationManager_->SetAnimation(type); }

	/// <summary>
	/// 音の再生
	/// </summary>
	/// <param name="type"></param>
	void SetPlaySound(PlayerSoundManager::AudioType type) { soundManager_->PlayAudio(type); }

	/// <summary>
	/// 音の停止
	/// </summary>
	/// <param name="type"></param>
	void SetStopSound(PlayerSoundManager::AudioType type) { soundManager_->StopAudio(type); };

	/// <summary>
	/// 入力方向を向く
	/// </summary>
	/// <returns>向きベクトルを返却</returns>
	Vector3 SetBody4Input();

#pragma region 状態管理とメンバ関数ポインタテーブル

	//プレイヤーの状態
	State behavior_ = State::Move;

	//状態群
	std::vector<std::unique_ptr<PlayerBaseBehavior>>behaviors_;

#pragma endregion

private://**プライベート変数**//

	//入力管理
	std::unique_ptr<PlayerInputManager>input_;

	//カメラ
	const Camera* camera_ = nullptr;

	//当たり判定
	std::unique_ptr<SphereCollider> collider_;

	//移動エフェクト
	std::unique_ptr<EffectMove>effectMove_;

	//攻撃エフェクト
	std::unique_ptr<EffectImpact>impactE_;

	//円の影
	std::unique_ptr<CircleShadow>circleShadow_;

	//プレイヤー関連のUI
	std::unique_ptr<PlayerUI>ui_;

	//音管理
	std::unique_ptr<PlayerSoundManager>soundManager_;

	//アニメーション管理
	std::unique_ptr<PlayerAnimationManager>animationManager_;

	//落下速度
	float fallSpd_ = 0.1f;

	//加算式落下加速度
	float addFallSpd_ = 0;
};