#pragma once
#include"IScene/IScene.h"
#include"Input/Input.h"
#include"Sprite/Sprite.h"
#include"Game/ScoreSaveManager/ScoreSaveManager.h"

//クリアシーン
class ClearScene : public IScene {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClearScene();
	~ClearScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private://**プライベート関数**//

	/// <summary>
	/// キル数分カウントを増やす処理
	/// </summary>
	void CountKill();

	/// <summary>
	/// 文字が段々浮かんでくる処理
	/// </summary>
	void FadeIn();

	/// <summary>
	/// 画面が暗くなって行く処理
	/// </summary>
	void BlackOut();

private:
	//入力関係
	Input* input_=nullptr;

	
	//各数字
	std::unique_ptr<Sprite>num1_;
	std::unique_ptr<Sprite>num10_;
	std::unique_ptr<Sprite>num100_;

	std::unique_ptr<Sprite>resultText_;
	std::unique_ptr<Sprite>backScreen_;

	std::unique_ptr<Sprite>sceneC_;

private://**プライベート変数**//

	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	//討伐数計算の処理が終わったか
	bool serchComplete_ = false;

	//透明度の値
	float alphaNum_ = 0;

	//目標時間
	const float addAlphaSec_ = 2.0f;

	//最大透明度
	const float screenmaxAlphaNum_ = 0.7f;

	//暗転時間
	float blackoutSec_ = 3.0f;

	//1フレームごとに計算する敵の数
	const float frameCountEnemy_ = 1.0f;
	//現在のカウント数
	float currentCountEnemy_ = 0;

	//スコアのデータ
	ScoreData scoreData_;

	//クリアBGMの配列番号
	int bgmClear_;

};