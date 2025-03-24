#pragma once
#include"Input/Input.h"


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