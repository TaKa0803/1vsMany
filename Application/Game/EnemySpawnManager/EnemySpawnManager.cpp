#include "EnemySpawnManager.h"
#include"DeltaTimer/DeltaTimer.h"

EnemyManager::EnemyManager()
{

	//エフェクト生成
	brokenBody_ = std::make_unique<BrokenBody>();

}

EnemyManager::~EnemyManager()
{
	//敵のデータを全削除
	enemies_.clear();
}

void EnemyManager::Initialize()
{
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
			if (points.spawnCount <= 0) {
				//カウントリセット
				points.spawnCount = maxSpawnCount_;

				//敵を出現
				std::unique_ptr<Enemy>newEnemy=std::make_unique<Enemy>();
				newEnemy->Initialize(points.obj->world_.GetWorldTranslate(),pWorld_);
			}
			else {
				points.spawnCount -= (float)DeltaTimer::deltaTime_;
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
