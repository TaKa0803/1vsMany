#pragma once
#include"Sprite/Sprite.h"

//ゲームの残り時間関係の処理
class CountTimer {

public://**パブリック関数**//

	CountTimer();
	~CountTimer() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// 画像の更新
	/// </summary>
	void SpriteUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// カウント終了フラグ
	/// </summary>
	/// <returns></returns>
	bool isCountEnd() { return isCountEnd_; };

private://**プライベート変数**//

	std::unique_ptr<Sprite>num1_;
	std::unique_ptr<Sprite>num10_;


private:

	//パラメータ群
	struct Parameter {

		float maxCount = 60;

	}param_;

	bool isCountEnd_ = false;
	float currentCount_ = 0;
};