#pragma once
#include"Game/EnemyManager/Enemy/Enemy.h"
#include"Game/EnemyManager/SpawnManager/EnemySpawnManager.h"
#include"Game/EnemyManager/ParameterManager/EnemyParameterManager.h"
#include"Sprite/Sprite.h"
#include"Game/BrokenBody/BrokenBody.h"
#include<memory>

//敵データの総合管理クラス
class EnemyManager {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager(const  EulerWorldTransform&playerWorld);
	~EnemyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 敵の当たり判定処理
	/// </summary>
	/// <param name="collider">プレイヤーコライダー</param>
	/// <param name="isPlayerATK">プレイヤー攻撃フラグ</param>
	/// <returns>攻撃ヒット処理を行ったか否か</returns>
	bool Collision(SphereCollider* collider, bool isPlayerATK);

	/// <summary>
	/// キル数の取得
	/// </summary>
	/// <returns></returns>
	int GetKillCount() const { return killCount_; };

private://**プライベート関数**//

	/// <summary>
	/// 敵の生成処理まとめ
	/// </summary>
	void SpawnEnemy();

	/// <summary>
	/// 敵データ群更新
	/// </summary>
	void UpdateEnemies();

	/// <summary>
	/// 数字スプライトのUVを設定
	/// </summary>
	void SetTxetureUV();

private://**プライベート変数**//

	//敵のステータス管理マネージャ(現在は未使用)
	std::unique_ptr<EnemyParameterManager>parameterManager_;
	
	//出現マネージャ
	std::unique_ptr<EnemySpawnManager>spawnManager_;

	//壊れた体のエフェクト
	std::unique_ptr<BrokenBody> brokenBody_;

	//敵のデータ群
	std::list<std::unique_ptr<Enemy>>enemies_;

	EulerWorldTransform textBaseWorld_;

	//各数字
	std::unique_ptr<Sprite>num1_;
	std::unique_ptr<Sprite>num10_;
	std::unique_ptr<Sprite>num100_;

private://**パラメータ変数**//

	//キル数
	int killCount_ = 0;

	//死亡音
	int breakSound_;

	//敵の数
	int enemyCount_ = 0;

	//デバッグ用敵の全削除フラグ
	bool clearEnemy_ = false;
};