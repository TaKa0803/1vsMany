#include "EnemySpawnManager.h"
#include"DeltaTimer/DeltaTimer.h"
#include"RandomNum/RandomNum.h"


#include<memory>

EnemySpawnManager::EnemySpawnManager(const EulerWorldTransform& playerWorld)
{
	playerWorld_ = &playerWorld;

	//デバッグ用に値を追加
	tree_.name_ = "出現データ";
	//各パラメータ追加
	tree_.SetValue("出現間隔", &maxSpawnSec_);
	tree_.SetValue("同時出現数", &maxSpawnNum_);
	tree_.SetValue("最大出現数", &maxSpawnCount_);
	tree_.SetValue("出現範囲", &maxSpawmWide_);

}

std::list<std::unique_ptr<Enemy>> EnemySpawnManager::SpawnEnemy(int enemyCount)
{
	//新しい敵データ作成
	std::list<std::unique_ptr<Enemy>>newEnemies;

	//生成フラグが無効の場合リターン
	if (!isSpawn_||enemyCount>=maxSpawnCount_) {
		return newEnemies;
	}

	//生成フラグを無効化
	isSpawn_ = false;

	//数分生成
	for (int i = 0; i < maxSpawnNum_; i++) {
		//敵のデータ作成
		std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
		
		//プレイヤー地点から一定距離離れて出現

		//誤差の値作成
		Vector3 differencePos = {
			RandomNumber::Get(-maxSpawmWide_, maxSpawmWide_),
			0,
			RandomNumber::Get(-maxSpawmWide_, maxSpawmWide_),
		};

		//誤差を含めた座標生成
		Vector3 pos = playerWorld_->GetWorldTranslate()+differencePos;

		//座標をセットして初期化
		newEnemy->Initialize(pos,playerWorld_);

		//データに追加
		newEnemies.emplace_back(std::move(newEnemy));
	}

	//データを渡す
	return std::move(newEnemies);
}


void EnemySpawnManager::Update()
{

	//敵の生成処理
	//生成フラグが有効の場合終了
	if (isSpawn_) {
		return;
	}

	//時間の加算
	currrentSpawnSec_ += (float)DeltaTimer::deltaTime_;
	//指定時間で処理
	if (currrentSpawnSec_ >= maxSpawnSec_) {		
		//時間をリセット
		currrentSpawnSec_ = 0;

		//生成フラグを有効
		isSpawn_ = true;
	}

}
