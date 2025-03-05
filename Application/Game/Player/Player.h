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

#include"Game/Player/Sound/PlayerSoundManager.h"
#include"Game/Player/Animation/PlayerAnimationManager.h"
#include<vector>





class Player :public GameObject {

public://**パブリック変数**//

	//基本パラメータ
	PlayerParameter parameter_;

public://**パブリック変数**//

	Player();
	~Player()=default;

	void Initialize();

	/// <summary>
	/// ゲーム関係の更新
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// モデルの更新
	/// </summary>
	void ObjectUpdate();

	void Draw();

	void DrawParticle();

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	void DebugWindow(const char* name);

	void SetCamera(Camera* camera) { camera_ = camera; }

	//押し戻し
	void OnCollisionBack(const Vector3& backV);

	SphereCollider* GetCollider() { return collider_.get(); }

	

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

	/// <summary>
	/// アニメーションのセット
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	void SetAnimation(PlayerAnimationManager::Animation type) { animationManager_->SetAnimation(type); }

	/// <summary>
	/// 音のセット
	/// </summary>
	/// <param name="type"></param>
	void SetSound(PlayerSoundManager::AudioType type) { soundManager_->PlayAudio(type); }

private://メンバ関数


	//移動
	void Move();

	void ModelRoop(const Vector3& velo);

#pragma region 状態管理とメンバ関数ポインタテーブル



	//プレイヤーの状態
	State behavior_ = State::Move;
	//状態リクエスト
	std::optional<State>behaviorRequest_ = std::nullopt;

	//状態群
	std::vector<std::unique_ptr<PlayerBaseBehavior>>behaviors_;

	//状態ごとの初期化テーブル
	static void (Player::* BehaviorInitialize[])();

	//状態ごとの更新テーブル
	static void (Player::* BehaviorUpdate[])();


	void InitializeMove();

	void InitializeATK();

	void InitializeHitAction();

	void InitializeSpecialATK();

	void UpdateMove();

	void UpdateATK();

	void UpdateHitAction();

	void UpdateSpecialATK();
#pragma endregion

private://**プライベート変数**//

	//入力
	Input* input_ = nullptr;

	//カメラ
	const Camera* camera_ = nullptr;

	//当たり判定
	std::unique_ptr<SphereCollider> collider_;

	//移動エフェクト
	std::unique_ptr<EffectMove>effectMove_;

	//プレイヤー関連のUI
	std::unique_ptr<PlayerUI>ui_;

	//音管理
	std::unique_ptr<PlayerSoundManager>soundManager_;

	//アニメーション管理
	std::unique_ptr<PlayerAnimationManager>animationManager_;

#pragma region モデルに関する

	//タグ軍

	#pragma endregion

#pragma region 影
	std::unique_ptr<InstancingGameObject>shadow;
#pragma endregion




	//落下速度
	float fallSpd_ = 0.1f;

	//加算式落下加速度
	float addFallSpd_ = 0;



	enum MoveState {
		StopS,
		MoveS,
		NoneS
	};

	MoveState moveState_ = NoneS;

#pragma region 攻撃に関する変数

	std::unique_ptr<EffectImpact>impactE_;

	//攻撃に関するデータ
	std::string atkDataPass_ = "resources/jsonfile/PlayerATKData.json";

	//グループ名
	std::string groupName_ = "BButtonATK";

	static const int itemNum = 2;

	//アイテムの名前
	std::string keyNames_[itemNum] = {
		"data",
		"ATKDerivation"
	};



#pragma endregion





};