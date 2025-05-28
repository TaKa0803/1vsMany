#include "JumpEnemyManager.h"
#include "GlobalVariable/Group/GlobalVariableGroup.h"
#include"InstancingModelManager/InstancingModelManager.h"

JumpEnemyManager::JumpEnemyManager()
{

	for (int i = 0; i < enemyNum_; i++) {
		//生成して追加
		jumpEnemies_.emplace_back(std::make_unique<JumpEnemy>());
	}


	//デバッグ用グループ作成
	std::unique_ptr<GVariGroup>	gvg = std::make_unique<GVariGroup>("JumpEnemies");

	gvg->SetTreeData(InstancingModelManager::GetInstance()->CreateAndGetTree("TitleEnemy","モデルデータ"));

	GvariTree enemyTree;
	enemyTree.name_ = "敵のデータ";

	int i = 1;
	for(auto&enemy : jumpEnemies_)
	{
		char num = char(i);
		std::string name = "敵:" + std::to_string(num);
		//敵の座標データを追加
		enemyTree.SetValue(name, &enemy->GetWorldTransform().translate_);
		i++;
	}

	gvg->SetTreeData(enemyTree);
}

void JumpEnemyManager::Update()
{
	for (auto& enemy : jumpEnemies_) {
		enemy->Update();
	}
}

void JumpEnemyManager::Draw()
{
	for (auto& enemy : jumpEnemies_) {
		enemy->Draw();
	}
}