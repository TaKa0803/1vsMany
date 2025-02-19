#pragma once
#include"Vector3.h"
#include"InstancingGameObject/InstancingGameObject.h"
#include"Game/Enemy/ALEnemy.h"
#include<vector>

//敵の出現管理マネージャ
class EnemySpawnManager {

public:

	EnemySpawnManager();
	~EnemySpawnManager();

	void Initialize();

	void Update();

	void Draw();

	void SetPWorld(const EulerWorldTransform& pWorld) { pWorld_ = &pWorld; };
public:

	std::vector<std::unique_ptr<ALEnemy>>&GetEnemies() { return enemies_; }

private:

	const EulerWorldTransform* pWorld_;

	struct SpawnObject {
		std::unique_ptr<InstancingGameObject>obj;
		int hp;
		float fixCount;
		float spawnCount;
		bool broken = false;
	};

	//敵生成
	std::vector<SpawnObject>spawnPoints_;

	std::vector<std::unique_ptr<ALEnemy>>enemies_;

	//最大HP
	int maxHp_;

	//修繕時間
	float maxFixCount_;

	//出現時間
	float maxSpawnCount_;

};