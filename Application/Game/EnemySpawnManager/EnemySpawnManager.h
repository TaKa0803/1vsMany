#pragma once
#include"Vector3.h"
#include"InstancingGameObject/InstancingGameObject.h"
#include"Game/Enemy/ALEnemy.h"
#include"Game/BrokenBody/BrokenBody.h"
#include<vector>
#include<list>

//敵の管理マネージャ
class EnemyManager {

public:

	EnemyManager();
	~EnemyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ゲーム側の更新
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetPWorld(const EulerWorldTransform& pWorld) { pWorld_ = &pWorld; };
public:

	std::list<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

	/// <summary>
	/// キルカウント取得
	/// </summary>
	/// <returns></returns>
	const int GetKillCount() { return killCount_; }
private:

	const EulerWorldTransform* pWorld_;

	struct SpawnObject {
		std::unique_ptr<InstancingGameObject>obj;
		int hp;
		float fixCount=0;
		float spawnCount=0;
		bool broken = false;
	};

	//敵生成
	std::vector<SpawnObject>spawnPoints_;

	std::list<std::unique_ptr<Enemy>>enemies_;

	//壊れた体のエフェクト
	std::unique_ptr<BrokenBody> brokenBody_;

	//スポーンのモデル
	std::string modelTag_ = "Flag";

private://**パラメータ**//

	//最大HP
	int maxHp_ = 10;

	//修繕時間
	float maxFixCount_ = 10.0f;

	//出現時間
	float maxSpawnSec_ = 5.0f;

	//同時出現数
	int maxSpawnNum_ = 1;

	//出現範囲
	float spawmWide_ = 5.0f;

	//キル数
	int killCount_ = 0;

	//出現最大数
	int maxSpawnCount_ = 200;
};