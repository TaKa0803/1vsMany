#include "EnemySpawnManager.h"
#include"DeltaTimer/DeltaTimer.h"
#include"RandomNum/RandomNum.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#include<memory>

EnemyManager::EnemyManager()
{
	//エフェクトクラス生成
	brokenBody_ = std::make_unique<BrokenBody>();

	//デバッグ用に値を追加
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("敵関係");
	//モニター値を追加
	gvg->SetMonitorValue("キル数", &killCount_);
	//各パラメータ追加
	gvg->SetValue("施設の最大HP", &maxHp_);
	gvg->SetValue("修繕時間/s", &maxFixCount_);
	gvg->SetValue("同時出現数", &maxSpawnNum_);
	gvg->SetValue("最大出現数", &maxSpawnCount_);
	//体が壊れる演出のツリー追加
	gvg->SetTreeData(brokenBody_->tree_);
}

EnemyManager::~EnemyManager()
{
	//敵のデータを全削除
	enemies_.clear();
}

void EnemyManager::Initialize()
{
	//仮でこちらで作成
	SpawnObject obj1{};
	//生成
	obj1.obj = std::make_unique<InstancingGameObject>();
	obj1.obj->Initialize(modelTag_);
	obj1.obj->world_.translate_ = { 100,0,100 };
	//パラメータをセット
	obj1.hp = maxHp_;
	//データ追加
	spawnPoints_.push_back(std::move(obj1));

	SpawnObject obj2{};
	//生成
	obj2.obj = std::make_unique<InstancingGameObject>();
	obj2.obj->Initialize(modelTag_);
	obj2.obj->world_.translate_ = { -100,0,100 };
	//パラメータをセット
	obj2.hp = maxHp_;
	//データ追加
	spawnPoints_.push_back(std::move(obj2));

	SpawnObject obj3{};
	//生成
	obj3.obj = std::make_unique<InstancingGameObject>();
	obj3.obj->Initialize(modelTag_);
	obj3.obj->world_.translate_ = { 100,0,-100 };
	//パラメータをセット
	obj3.hp = maxHp_;
	//データ追加
	spawnPoints_.push_back(std::move(obj3));

	SpawnObject obj4{};
	//生成
	obj4.obj = std::make_unique<InstancingGameObject>();
	obj4.obj->Initialize(modelTag_);
	obj4.obj->world_.translate_ = { -100,0,-100 };
	//パラメータをセット
	obj4.hp = maxHp_;
	//データ追加
	spawnPoints_.push_back(std::move(obj4));
}

void EnemyManager::GameUpdate()
{

	//各出現施設の更新
	for (auto& points : spawnPoints_) {

		//施設の復旧中なら
		if (points.broken) {
			//カウント進行
			points.fixCount -= (float)DeltaTimer::deltaTime_;
			//カウント満了で修復
			if (points.fixCount <= 0) {
				points.fixCount = 0;
				points.broken = false;
			}
		}
		else {
			
			//敵の生成処理
			if (points.spawnCount >= maxSpawnSec_) {
				//カウントリセット
				points.spawnCount = 0;

				//出現数制限
				if (enemies_.size() >= maxSpawnCount_) {
					break;
				}

				//指定数出現
				for (int i = 0; i < maxSpawnNum_; i++) {
					//出る場所を決定
					Vector3 newWorldPos;
					newWorldPos = points.obj->world_.GetWorldTranslate() + (Vector3(RandomNumber::Get(-1, 1), 0, RandomNumber::Get(-1, 1)).SetNormalize() * spawmWide_);

					//敵を生成
					std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
					newEnemy->Initialize(newWorldPos, pWorld_);
					enemies_.push_back(std::move(newEnemy));
				}
			}
			else {
				points.spawnCount += (float)DeltaTimer::deltaTime_;
			}

		}

	}

	//敵の更新と削除処理
	enemies_.remove_if([&](std::unique_ptr<Enemy>&enemy) {

		//死亡フラグ取得
		if (!enemy->GetDead()) {
			//更新
			enemy->Update();
			//削除しない
			return false;
		}
		else {
			//エフェクトを生成
			brokenBody_->EffectOccurred(enemy->world_, 10);

			//キルカウント増加
			killCount_++;

			//リストから削除
			return true;
		}

		});



}

void EnemyManager::ObjectUpdate()
{
	//建物の更新
	for (auto& spawn : spawnPoints_) {

		spawn.obj->Update();
	}

	//エフェクトの更新
	brokenBody_->Update();
}

void EnemyManager::Draw()
{
	brokenBody_->Draw();

	//建物
	for (auto& spawn : spawnPoints_) {
		spawn.obj->Draw();
	}

	//敵
	for (auto& enemy : enemies_) {
		if (!enemy->GetDead()) {
			enemy->Draw();
		}
	}
}
