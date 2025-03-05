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
	/// 攻撃入力取得
	/// </summary>
	/// <returns></returns>
	bool GetAttackInput();

private://**パブリック変数**//

	//入力
	Input* input_=nullptr;

};