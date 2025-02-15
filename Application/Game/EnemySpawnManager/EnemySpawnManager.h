#pragma once

//敵の出現管理マネージャ
class EnemySpawnManager {

public:

	EnemySpawnManager();
	~EnemySpawnManager() = default;

	void Initialize();

	void Update();

	void Draw();

};