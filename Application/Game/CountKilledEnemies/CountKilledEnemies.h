#pragma once
#include"Sprite/Sprite.h"


class CountKilledEnemies {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CountKilledEnemies();
	~CountKilledEnemies()=default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(int killNum);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public://**ゲッター**//

	/// <summary>
	/// ツリーの取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; };

	/// <summary>
	/// 終了フラグ取得
	/// </summary>
	/// <returns></returns>
	bool CheckComplete() { return isSerchComplete_; };

private://**プライベート関数**//

	/// <summary>
	/// 数字スプライトのUV座標を変更
	/// </summary>
	void SetUV(int count);

private://**プライベート変数**//

	//各数字
	std::unique_ptr<Sprite>num1_;
	std::unique_ptr<Sprite>num10_;
	std::unique_ptr<Sprite>num100_;

	//文字スプライト
	std::unique_ptr<Sprite>resultText_;

	//討伐数計算の処理が終わったか
	bool isSerchComplete_ = false;

	//討伐数
	int killNum_ = 0;

	//1フレームごとに計算する敵の数
	const float frameCountEnemy_ = 1.0f;

	//現在のカウント数
	float currentCountEnemy_ = 0;

private://**

	GvariTree tree_;

};