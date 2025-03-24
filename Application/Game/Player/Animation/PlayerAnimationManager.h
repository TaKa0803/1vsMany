#pragma once
#include"Game/BaseManager/BaseManager.h"
#include"SingleGameObjects/Model.h"
#include<string>

//プレイヤーのアニメーション管理
class PlayerAnimationManager :public BaseManager {

public://**パブリック変数
	enum Animation {
		ATK_Punch,
		ATK_Kick,
		ATK_Drill,
		WAIT,
		WALK,
		CountAnimation
	};

public://**パブリック関数

	PlayerAnimationManager(Model*model);
	~PlayerAnimationManager()=default;

	/// <summary>
	/// アニメーション再生
	/// </summary>
	/// <param name="anime"></param>
	void SetAnimation(Animation anime);

private://**プライベート変数**//

	//プレイヤーモデルのポインタ
	Model* model_;
	
	//アニメーション名
	std::string animeName_[5] = {
		"ATK1",
		"ATK2",
		"ATK3",
		"wait",
		"walk"
	};

	//変わりきるまでの時間
	float transitionSeconds_[CountAnimation] = { 1.0f };

	//ループするか
	bool isLoops_[CountAnimation] = { false };

	//アニメーション再生倍率
	float multiplyAnimationSpeeds_[CountAnimation] = { 1.0f };
};