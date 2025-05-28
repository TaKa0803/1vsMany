#pragma once
#include"Title/JumpEnemy/JumpEnemy.h"

//ジャンプする敵の動き制御
class JumpEnemyManager
{
public://**パブリック関数**//
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	JumpEnemyManager();
	~JumpEnemyManager() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private://**プライベート変数**//

	//敵の数
	int enemyNum_ = 3;

	//時間
	float sec_ = 0;

	//ジャンプする敵の配列
	std::vector<std::unique_ptr<JumpEnemy>> jumpEnemies_;

private://**パラメータ変数**//

	//ジャンプする敵のデータ
	JumpEnemyData jumpEnemyData_;

	//ジャンプする動きの間隔
	float jumpWaitSec_ = 1.0f;

	//ジャンプ処理を行うかどうか
	bool isJump_ = true;
};