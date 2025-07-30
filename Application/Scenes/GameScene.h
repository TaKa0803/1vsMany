#pragma once

#include "IScene/IScene.h"

#include<iostream>
#include<list>

#include"Input/Input.h"
#include"Camera/Camera.h"

#include"Game/Player/Player.h"
#include"Game/Plane/Plane.h"

#include"Sprite/Sprite.h"

#include"ParticleData/ParticleManager.h"

#include"Game/CountTimer/CountTimer.h"
#include"Game/EnemyManager/EnemyManager.h"
#include"Game/FollowCamera/FollowCamera.h"
#include"Game/ScoreSaveManager/ScoreSaveManager.h"
#include"Game/Transition/Transition.h"

//ゲームシーン
class GameScene : public IScene {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();
	~GameScene()=default;

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
	/// デバッグウィンドウ処理
	/// </summary>
	void DebugWindows();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Collision();

	/// <summary>
	/// シーン変更処理
	/// </summary>
	void SceneChange();

	/// <summary>
	/// UIの描画
	/// </summary>
	void UIDraw();

private://**遷移処理**//

	//ゲームシーン内の状態
	enum GameSceneBehavior {
		Other2ThisScene,	//他シーンからここ
		ThisScene,			//現在のシーン更新
		This2Other,			//現在のシーンから他へ
		CountScene
	}scene_=Other2ThisScene;

	//状態リクエスト
	std::optional<GameSceneBehavior>sceneRequest_ = std::nullopt;

	//状態ごとの初期化テーブル
	static void (GameScene::* BehaviorInitialize[])();

	//状態ごとの更新テーブル
	static void (GameScene::* BehaviorUpdate[])();

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

	//キー入力
	Input* input_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player>player_;
	
	//敵マネージャ
	std::unique_ptr<EnemyManager>enemyManager_;
	
	//地面
	std::unique_ptr<Plane>plane_;

	//時間カウント
	std::unique_ptr<CountTimer>countTimer_;

	//カメラ処理
	std::unique_ptr<FollowCamera>followCamera_;

	//スコアの保存
	std::unique_ptr<ScoreSaveManager>scoreSaveManager_;

	//遷移クラス
	std::unique_ptr<Transition>transition_;


	
	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	//BGMの配列番号
	int bgmGame_;

	
};