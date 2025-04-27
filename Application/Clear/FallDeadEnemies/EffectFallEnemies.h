#pragma once
#include"InstancingModelManager/InstancingModelManager.h"
#include"Clear/FallDeadEnemies/FallEnemy/FallEnemy.h"
#include<list>

//敵が落ちまくるエフェクト
class EffectFallEnemies {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectFallEnemies();
	~EffectFallEnemies()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="count">敵の数</param>
	void Initialize(const int count);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵を生成
	/// </summary>
	/// <param name="num">生成する数</param>
	void EmitEnemies();


private://**プライベート変数**//

	//敵モデルタグ名
	std::string modelName_ = "FallEnemy";

	//敵の数
	int count_ = 0;

	//生成数
	int spawnCount_ = 0;

	//生成間隔
	float spawnSec_ = 0.0f;

	//座標群
	std::list<std::unique_ptr<FallEnemy>> enemies_;

private://**パラメータ変数**//

	//出現位置
	Vector3 spawnCenterPosition_ = { 0,0,0 };	
	
	//出現範囲
	float spawnRange_ = 1.0f;	

	//最小最大移動速度
	Vector2 speeds_ = { 0.01f,0.1f };

	//最小最大落下速度
	Vector2 fallSpeeds_ = { 0.1f,1.0f };

	//最大回転速度
	float rotateSpeed_ = 0.1f;

	//死亡時間
	float maxDeadSec_ = 1.0f;	

	//最大生成間隔
	float maxSpawnSec_ = 1.0f;

	//同時生成数
	int spawnNum_ = 1;

	//最小最大反発力
	Vector2 repulsion_ = { 0.1f,0.8f };

	//反発高度
	float repulsionHeight_ = 0.1f;


private://**デバッグ用

	//敵を全削除
	bool clearEnemy_ = false;
};