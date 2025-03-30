#pragma once

#include"InstancingModelManager/InstancingModelManager.h"

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
	void EmitEnemies(int num);

};