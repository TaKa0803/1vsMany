#include "JumpEnemyManager.h"
#include "GlobalVariable/Group/GlobalVariableGroup.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"DeltaTimer/DeltaTimer.h"
#include"RandomNum/RandomNum.h"

JumpEnemyManager::JumpEnemyManager()
{

	for (int i = 0; i < enemyNum_; i++) {
		//生成して追加
		jumpEnemies_.emplace_back(std::make_unique<JumpEnemy>(jumpEnemyData_));
	}


	//デバッグ用にパラメータを設定
	std::unique_ptr<GVariGroup>	gvg = std::make_unique<GVariGroup>("JumpEnemies");

	//モデルのデータを設定
	gvg->SetTreeData(InstancingModelManager::GetInstance()->CreateAndGetTree("TitleEnemy","モデルデータ"));
	gvg->SetMonitorValue("jump行為", &isJump_);
	gvg->SetValue("jumpの間隔",&jumpWaitSec_);
	//jumpしてる敵の情報
	GvariTree enemyTree;
	enemyTree.name_ = "敵のデータ";
	enemyTree.SetValue("jump初速度", &jumpEnemyData_.speed);
	enemyTree.SetValue("加速度", &jumpEnemyData_.acceleration);

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
	//時間加算
	sec_ += (float)DeltaTimer::deltaTime_;
	//時間経過でjump
	if (sec_ >= jumpWaitSec_) {
		//時間をリセット
		sec_ = 0;
		//処理を行うフラグがONの場合
		if (isJump_) {
			//敵をランダムに選んでjump
			int num = (int)RandomNumber::Get(0, 3);
			if (num == 3)num = 2; //3は存在しないので2にする
			//選んだ敵にjumpを設定
			jumpEnemies_[num]->SetJump();
		}
	}

	//敵の更新
	for (auto& enemy : jumpEnemies_) {
		enemy->Update();
	}
}

void JumpEnemyManager::Draw()
{
	//敵の描画
	for (auto& enemy : jumpEnemies_) {
		enemy->Draw();
	}
}