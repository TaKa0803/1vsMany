#include "PlayerInputManager.h"

PlayerInputManager::PlayerInputManager()
{
	input_ = Input::GetInstance();
	//デッドライン設定
	input_->SetDeadLine(0.3f);
}

const Vector3 PlayerInputManager::GetMoveInput()
{
	//入力データ保存変数
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
	//攻撃入力の
	int isAttackInput = 0;

	//キー入力取得
	isAttackInput = input_->TriggerKey(DIK_Z);
	//パッド入力取得
	isAttackInput += input_->IsTriggerButton(kButtonB);

	return (bool)isAttackInput;
}
