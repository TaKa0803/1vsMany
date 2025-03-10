#include "PlayerAnimationManager.h"

PlayerAnimationManager::PlayerAnimationManager(Model* model)
{
	//モデルデータ参照
	model_ = model;
	//アニメーション設定を有効にする
	model_->SetAnimationActive(true);

	//デバッグ用ツリー設定
	tree_.name_ = "アニメーションデータ";

	//ツリーデータ群
	GvariTree trees[CountAnimation];

	//データ群にパラメータをセット
	int index = 0;
	for (auto& tree : trees) {
		tree.name_ = animeName_[index];
		tree.SetValue("変化しきる時間", &transitionSeconds_[index]);
		tree.SetValue("ループフラグ", &isLoops_[index]);
		tree.SetValue("再生倍率", &multiplyAnimationSpds_[index]);

		//データ追加
		tree_.SetTreeData(tree);

		//インデックス増加
		index++;
	}


}

void PlayerAnimationManager::SetAnimation(Animation anime)
{
	//配列番号取得
	size_t num = (size_t)anime;
	//アニメーション設定を反映
	model_->ChangeAnimation(animeName_[anime], transitionSeconds_[num]);
	//ループフラグの設定
	model_->SetAnimationRoop(isLoops_[num]);
	//アニメーション再生倍率の変更
	model_->animationRoopSecond_ = multiplyAnimationSpds_[num];
}
