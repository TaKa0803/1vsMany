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

const PlayerInputParameter PlayerInputManager::GetAttackInput()
{
	//返す変数生成
	PlayerInputParameter parameter{};

	//入力取得
	bool inputB = GetAttackInputB();
	bool inputA = GetAttackInputA();

	//どちらも入力があるかどちらもない場合
	if ((inputB && inputA)|| (!inputB && !inputA)) {
		//何もしない
	}
	else {
		//どちらかの入力がある場合
		//値をセット
		parameter.inputA = inputA;
		parameter.inputB = inputB;
	}

	return parameter;
}

bool PlayerInputManager::GetAttackInputB()
{
	//攻撃入力の
	int isAttackInput = 0;

	//キー入力取得
	isAttackInput = input_->TriggerKey(DIK_Z);
	//パッド入力取得
	isAttackInput += input_->IsTriggerButton(kButtonB);

	return (bool)isAttackInput;
}

bool PlayerInputManager::GetAttackInputA()
{
	//攻撃入力の
	int isAttackInput = 0;

	//キー入力取得
	isAttackInput = input_->TriggerKey(DIK_X);
	//パッド入力取得
	isAttackInput += input_->IsTriggerButton(kButtonX);

	return (bool)isAttackInput;
}
