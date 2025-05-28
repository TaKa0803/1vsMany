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

private:

	//敵の数
	int enemyNum_ = 3;

	//ジャンプする敵の配列
	std::vector<std::unique_ptr<JumpEnemy>> jumpEnemies_;
};