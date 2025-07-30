#pragma once
#include"IScene/IScene.h"
#include"Input/Input.h"
#include"Sprite/Sprite.h"
#include"Game/ScoreSaveManager/ScoreSaveManager.h"
#include"Clear/CountKilledEnemies/CountKilledEnemies.h"
#include"Game/Transition/Transition.h"
#include"Clear/FallDeadEnemies/EffectFallEnemies.h"
#include"Camera/Camera.h"

//クリアシーン
class ClearScene : public IScene {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClearScene();
	~ClearScene()=default;

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



private://**シーン関係**//

	//ゲームシーン内の状態
	enum ClearSceneBehavior {
		Other2ThisScene,	//他シーンからここへ
		ThisScene,			//現在のシーン更新
		This2Other,			//現在のシーンから他へ
		CountScene
	}scene_;

	//状態リクエスト
	std::optional<ClearSceneBehavior>sceneRequest_ = std::nullopt;

	//現在の状態
	ClearSceneBehavior sceneBehavior_ = Other2ThisScene;

	//状態ごとの初期化テーブル
	static void (ClearScene::* BehaviorInitialize[])();

	//状態ごとの更新テーブル
	static void (ClearScene::* BehaviorUpdate[])();

	//シーンエントリー時の処理
	void InitOther2This();
	//ゲーム本編の状態
	void InitThis();
	//シーン離脱状態の処理
	void InitThis2Other();

	//各状態更新
	//シーンエントリー更新
	void UpdateOther2This();
	//シーン更新
	void UpdateThis();
	//シーン離脱更新
	void UpdateThis2Other();

private://**プライベート変数**//
	//入力関係
	Input* input_=nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	//背景
	std::unique_ptr<Sprite>beforeScene_;

	//敵の討伐数をカウントするクラス
	std::unique_ptr<CountKilledEnemies>countKilledEnemies_;

	//遷移クラス
	std::unique_ptr<Transition>transition_;

	//敵が落ちていく演出
	std::unique_ptr<EffectFallEnemies>fallEnemies_;

private://**プライベート変数**//

	//スコアのデータ
	ScoreData scoreData_;

	//クリアBGMの配列番号
	int bgmClear_;
};