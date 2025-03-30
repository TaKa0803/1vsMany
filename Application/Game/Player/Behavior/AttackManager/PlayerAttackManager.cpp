#include "PlayerAttackManager.h"
#include"Game/Player/Player.h"
#include"Game/Player/Behavior/AttackManager/BButtonComboManager/PlayerBButtonComboManager.h"
#include<cassert>

PlayerAttackManager::PlayerAttackManager()
{
	//各ボタンの攻撃管理クラスの配列のサイズをセット
	buttonManagers_.resize((size_t)InputType::CountInputType);

	//各マネージャをセット
	buttonManagers_[(size_t)InputType::B] = std::make_unique<PlayerBButtonManager>(player_);

	//デバッグ用にパラメータをツリーにセット
	tree_.name_ = "攻撃管理";
	//デバッグツリーをセット
	//for (int i = 0; i < (int)InputType::CountInputType; i++) {
		//tree_.SetTreeData(buttonManagers_[i]->tree_);
	//}

	//現在はBボタンないので終わり
	tree_.SetTreeData(buttonManagers_[(size_t)InputType::B]->tree_);
}

void PlayerAttackManager::Init()
{
	//コンボ数を初期化
	player_->baseParameter_.comboCount = 0;
	//移動量を初期化
	player_->baseParameter_.velocity.SetZero();

	//ボタンの攻撃管理クラスを初期化
	const auto& data = player_->inputParameter_;
	//どちらかのタイプを指定
	if (data.inputA) {
		type_ = InputType::A;
	}
	else if (data.inputB) {
		type_ = InputType::B;
	}
	else {
		//どちらもfalseはおかしい
		assert(false);
	}

	//現在はBのみ
	type_ = InputType::B;

	//タイプに合わせて初期化
	buttonManagers_[(size_t)type_]->Init();

}

void PlayerAttackManager::Update()
{
	//入力取得
	//プレイヤーの攻撃データ取得
	bool& isEnd = PlayerBaseAttack::GetAttackData();
	//入力情報を取得
	auto& input = player_->GetInput()->GetAttackInput();

	//入力がないか同時押しの場合
	if ((!input.inputA && !input.inputB)||(input.inputA&&input.inputB)) {
	    //何もしない
	}
	else {
		//値をセット
		isInputA_ = input.inputA;
		isInputB_ = input.inputB;
	}



	//終了フラグが立っているなら
	if (isEnd) {
		//初期化
		isEnd = false;

		//カウント加算
		int attackCount = ++player_->baseParameter_.comboCount;

		//入力フラグが立っているなら処理もう一回攻撃
		if (isInputA_) {
			//タイプをセット	
			type_ = InputType::A;
		}
		else if (isInputB_) {
			//タイプをセット
			type_ = InputType::B;
		}
		else {
			//入力がない場合
			behaviorRequest_ = State::Move;
			//早期リターン
			return;
		}

		//現在はBのみ
		type_ = InputType::B;



		//最大数の場合攻撃状態を終了
		if (attackCount >= attackMax_) {
			behaviorRequest_ = State::Move;
			//早期リターン
			return;
		}
		else {
			//攻撃管理クラスを初期化
			buttonManagers_[(size_t)type_]->Init();
			//フラグ初期化
			isInputA_ = false;
			isInputB_ = false;
		}
	}

	//ボタンの攻撃管理クラスを更新
	buttonManagers_[(size_t)type_]->Update();

}

