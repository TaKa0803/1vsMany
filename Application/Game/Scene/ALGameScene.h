#pragma once

#include "IScene/IScene.h"

#include<iostream>
#include<list>

#include"Input/Input.h"
#include"Camera/Camera.h"

#include"Game/Player/Player.h"
#include"Game/Plane/Plane.h"
#include"Game/Enemy/ALEnemy.h"

#include"Sprite/Sprite.h"
#include"Game/BrokenBody/BrokenBody.h"
#include"ParticleData/ParticleManager.h"

#include"Game/CountTimer/CountTimer.h"
#include"Game/EnemySpawnManager/EnemySpawnManager.h"
#include"Game/FollowCamera/FollowCamera.h"

class GameScene : public IScene {

public:

	GameScene();

	~GameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	void DebugWindows();

	void Collision();

	void SceneChange();

	void UIDraw();

private://**遷移処理**//

	//ゲームシーン内の状態
	enum GameSceneBehavior {
		Other2ThisScene, //他シーンからここの
		ThisScene,
		This2Other,
		CountScene
	}scene_;

	//状態リクエスト
	std::optional<GameSceneBehavior>sceneRequest_ = std::nullopt;

	//状態ごとの初期化テーブル
	static void (GameScene::* BehaviorInitialize[])();

	//状態ごとの更新テーブル
	static void (GameScene::* BehaviorUpdate[])();

	void InitOther2This();

	void InitThis();

	void InitThis2Other();

	void UpdateOther2This();

	void UpdateThis();

	void UpdateThis2Other();

private:
	//キー入力
	Input* input_ = nullptr;
	//カメラクラス
	Camera* camera_;



	///以下ゲーム実装
	std::unique_ptr<ALPlayer>player_;

	std::unique_ptr<Plane>plane_;

	//時間カウント
	std::unique_ptr<CountTimer>countTimer_;

	//敵出現マネージャ
	std::unique_ptr<EnemyManager>enemySpawnManager_;

	//カメラ処理
	std::unique_ptr<FollowCamera>followCamera_;

	int limitMinute = 0;
	const int maxmilitMinute = 60 * 60;
	








	

	//シーンチェンジ用
	std::string white = "resources/Texture/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;

	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	//シーンが変わりきる時間
	const float maxSceneChangeSec_ = 1.0f;

	//経過時間カウント
	float currentSceneXhangeSec_ = 0;

	//BGMの配列番号
	int bgmGame_;

	std::unique_ptr<ParticleManager>ATKHitPerticle_;
};