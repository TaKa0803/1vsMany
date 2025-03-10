#pragma once
#include"SingleGameObjects/Model.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"
#include<string>

//プレイヤーのアニメーション管理
class PlayerAnimationManager {

public://**パブリック変数
	enum Animation {
		ATK1,
		ATK2,
		ATK3,
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

	/// <summary>
	/// ツリーの取得
	/// </summary>
	/// <returns>ツリーの参照データ</returns>
	GvariTree& GetTree() { return tree_; };

private://**プライベート変数**//

	//プレイヤーモデルのポインタ
	Model* model_;

	//パラメータツリー
	GvariTree tree_;
	
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
	float multiplyAnimationSpds_[CountAnimation] = { 1.0f };
};