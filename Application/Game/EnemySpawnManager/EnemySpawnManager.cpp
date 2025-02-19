#include "EnemySpawnManager.h"
#include"DeltaTimer/DeltaTimer.h"

EnemySpawnManager::EnemySpawnManager()
{



}

EnemySpawnManager::~EnemySpawnManager()
{
	enemies_.clear();
}

void EnemySpawnManager::Initialize()
{



}

void EnemySpawnManager::Update()
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
			if (points.spawnCount <= 0) {
				//カウントリセット
				points.spawnCount = maxSpawnCount_;

				//敵を出現
				std::unique_ptr<ALEnemy>newEnemy=std::make_unique<ALEnemy>();
				newEnemy->Initialize(points.obj->world_.GetWorldTranslate(),pWorld_);
			}
			else {
				points.spawnCount -= (float)DeltaTimer::deltaTime_;
			}

		}

	}

	for (auto& enemy : enemies_) {
		if (!enemy->GetDead()) {
			enemy->Update();
		}
	}

}

void EnemySpawnManager::Draw()
{

	//敵
	for (auto& enemy : enemies_) {
		if (!enemy->GetDead()) {
			enemy->Draw();
		}
	}
}
