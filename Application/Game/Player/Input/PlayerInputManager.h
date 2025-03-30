#pragma once
#include"Input/Input.h"

//入力パラメータ
struct PlayerInputParameter {
	//ボタンフラグB
	float inputB = false;
	//ボタンフラグA
	float inputA = false;
};

class PlayerInputManager {

public://**パブリック関数

	PlayerInputManager();
	~PlayerInputManager() = default;

	/// <summary>
	/// 移動入力取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetMoveInput();

	/// <summary>
	/// 攻撃入力取得
	/// </summary>
	const PlayerInputParameter GetAttackInput();

private://**プライベート関数**//

	/// <summary>
	/// 攻撃入力Bボタン（直線タイプ）取得
	/// </summary>
	/// <returns></returns>
	bool GetAttackInputB();

	/// <summary>
	/// 攻撃入力Aボタン（範囲タイプ）取得
	/// </summary>
	/// <returns></returns>
	bool GetAttackInputA();

private://**パブリック変数**//

	//入力
	Input* input_ = nullptr;

};