#include "PlayerInputManager.h"

PlayerInputManager::PlayerInputManager()
{
	input_ = Input::GetInstance();
}

const Vector3 PlayerInputManager::GetMoveInput()
{
	Vector3 move;

	//キー入力取得
	move = input_->GetWASD();
	//パッド入力取得
	move += input_->GetjoyStickLVec3();

	//正規化
	move.SetNormalize();

	return move;
}

bool PlayerInputManager::GetAttackInput()
{
	int isAttackInput = 0;

	//キー入力取得
	isAttackInput = input_->TriggerKey(DIK_Z);
	//パッド入力取得
	isAttackInput += input_->IsTriggerButton(kButtonB);

	return (bool)isAttackInput;
}
