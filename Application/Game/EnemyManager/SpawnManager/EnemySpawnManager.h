#pragma once
#include"Game/EnemyManager/Enemy/Enemy.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"
#include<list>

//敵の管理マネージャ
class EnemySpawnManager {

public:

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
	std::list<std::unique_ptr<Enemy>> SpawnEnemy(int enemyCount);

	/// <summary>
	/// ツリーの取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; };

private://**プライベート変数**//

	//プレイヤーのワールドデータ
	const EulerWorldTransform* playerWorld_;

	//出現間隔時間
	float currrentSpawnSec_=0;
	
	//デバッグ用のパラメータ保存ツリー
	GvariTree tree_;

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