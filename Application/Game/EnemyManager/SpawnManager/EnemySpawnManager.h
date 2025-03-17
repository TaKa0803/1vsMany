#pragma once
#include"Game/BaseManager/BaseManager.h"
#include"Game/EnemyManager/Enemy/Enemy.h"
#include"Game/EnemyManager/ParameterManager/EnemyParameterManager.h"
#include<list>

//敵の管理マネージャ
class EnemySpawnManager :public BaseManager{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerWorld">プレイヤーワールドポインタ</param>
	EnemySpawnManager(const EulerWorldTransform&playerWorld);
	~EnemySpawnManager()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 敵の生成処理
	/// </summary>
	/// <param name="enemyCount">敵の数</param>
	/// <returns>追加の敵データ</returns>
	std::list<std::unique_ptr<Enemy>> SpawnEnemy(int enemyCount,EnemyParameters parameters);

private://**プライベート変数**//

	//プレイヤーのワールドデータ
	const EulerWorldTransform* playerWorld_;

	//出現間隔時間
	float currrentSpawnSec_=0;

private://**パラメータ**//

	//出現間隔
	float maxSpawnSec_ = 5.0f;

	//同時出現数
	int maxSpawnNum_ = 1;

	//出現最大数
	int maxSpawnCount_ = 20;

	//最大出現範囲
	float maxSpawmWide_ = 30.0f;

	//最小出現範囲
	float minSpawnWide_ = 5.0f;

	//生成フラグ
	bool isSpawn_ = false;
};