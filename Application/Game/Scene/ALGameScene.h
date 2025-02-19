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

class ALGameScene : public IScene {

public:

	ALGameScene();

	~ALGameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	void DebugWindows();

	void Collision();

	void SceneChange();

	void UIUpdateDraw();

	void ClearUIUpdate();

	void LimitUI();

	void ShakeStart(int count);

	void CameraShake();
private:
	//キー入力
	Input* input_ = nullptr;
	//カメラクラス
	Camera* camera_;

	float xrotateNum = 0.05f;
	float yrotatenum = 0.02f;

	///以下ゲーム実装
	std::unique_ptr<ALPlayer>player_;

	std::unique_ptr<Plane>plane_;

	//時間カウント
	std::unique_ptr<CountTimer>countTimer_;

	//敵出現マネージャ
	std::unique_ptr<EnemySpawnManager>enemySpawnManager_;

	int limitMinute = 0;
	const int maxmilitMinute = 60 * 60;
	
	enum Scene {
		Game,
		Clear,
	};

	Scene scene_ = Game;










	std::unique_ptr<Sprite>num100_;

	std::unique_ptr<Sprite>backScreen_;
	std::unique_ptr<Sprite>resultText_;

	float alphaNum_ = 0;

	const float addAlphaNum_ = 120.0f;

	const float screenmaxAlphaNum_ = 0.7f;


	//1フレームごとに計算する敵の数
	const float frameCountEnemy_ = 1.0f;

	float nowFrameCountEnemy_ = 0;

	bool serchComplete_ = false;


	BrokenBody* brokenBody_;

	//シーンチェンジ用
	std::string white = "resources/Texture/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;

	//シーン転換後の処置
	bool preSceneChange_ = false;

	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	const float maxSceneChangeCount_ = 60;

	float sceneXhangeCount_ = 0;

	int bgmGame_;

	int bgmClear_;

	bool isShake_ = false;
	int cameraShakeCount_=0;
	Vector2 shakenum = {0,0};
	Vector3 tempP_;

	std::unique_ptr<ParticleManager>peM_;
};