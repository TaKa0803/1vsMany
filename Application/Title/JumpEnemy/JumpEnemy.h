#pragma once

//画面内でジャンプする敵
class JumpEnemy {
public:

	JumpEnemy();
	~JumpEnemy()=default;

	//更新
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
};