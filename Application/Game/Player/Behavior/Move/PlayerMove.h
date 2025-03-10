#pragma once
#include"Game/Player/Behavior/PlayerBaseBehavior.h"

class PlayerMove : public PlayerBaseBehavior {

public://**パブリック関数

	PlayerMove()=default;
	~PlayerMove()=default;


	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private:

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 遷移処理
	/// </summary>
	void SceneChange();

	/// <summary>
	/// アニメーション変更処理
	/// </summary>
	void ChangeAnimation();

private://**プライベート変数

	//移動速度
	float spd_ = 0.5f;


};