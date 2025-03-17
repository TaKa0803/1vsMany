#include "EnemyParameterManager.h"
#include"InstancingModelManager/InstancingModelManager.h"

EnemyParameterManager::EnemyParameterManager()
{
	//アニメーション設定
	InstancingModelManager* iMM = InstancingModelManager::GetInstance();
	iMM->SetAnimeName(stayAnimationModelTag_, "wait");
	iMM->SetAnimeName(moveAnimationModelTag_, "walk");



	//デバッグ用にパラメータ設定
	tree_.name_="ステータス";
	tree_.SetValue("最大速度までの経過時間", &parameters_.maxSpeedSec);
	tree_.SetValue("個体間での最大速度", &parameters_.maxSpeed);
	tree_.SetValue("個体間での最小速度", &parameters_.minSpeed);
	tree_.SetValue("落下速度", &parameters_.fallSpeed);
	tree_.SetValue("追跡停止距離", &parameters_.stopFollowRange);
	tree_.SetValue("追跡開始距離", &parameters_.startFollowRange);
	tree_.SetValue("加速度", &parameters_.acceraletion);
	tree_.SetValue("吹っ飛び量", &parameters_.hitSpeed);
	tree_.SetValue("最大体力", &parameters_.hp);

	//使用するモデルのデバッグ処理
	tree_.SetTreeData(iMM->CreateAndGetTree(stayAnimationModelTag_,"停止モデル"));
	tree_.SetTreeData(iMM->CreateAndGetTree(moveAnimationModelTag_, "移動モデル"));

	//ツリーにまとめて追加
	GvariTree animationTree;
	animationTree.name_ = "アニメーション関係";
	animationTree.SetValue("待機アニメーション再生倍率", &stayAnimationMultiplySpeed);
	animationTree.SetValue("移動アニメーション再生倍率", &moveAnimationMultiplySpeed);
	tree_.SetTreeData(animationTree);


}

void EnemyParameterManager::SetAnimationParameter() const
{
	//各アニメーションモデルタグに倍率を設定する
	InstancingModelManager* iMM = InstancingModelManager::GetInstance();
	iMM->SetAnimationRoopFrame(stayAnimationModelTag_, stayAnimationMultiplySpeed, true);
	iMM->SetAnimationRoopFrame(moveAnimationModelTag_, moveAnimationMultiplySpeed, true);

}
