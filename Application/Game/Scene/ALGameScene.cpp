#include"ALGameScene.h"

#include<imgui.h>

#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"RandomNum/RandomNum.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"

//#include"ColliderOBB/OBBCollider.h"
#include"DeltaTimer/DeltaTimer.h"

GameScene::GameScene() {
	input_ = Input::GetInstance();

	camera_ = Camera::GetInstance();
	player_ = std::make_unique<ALPlayer>();

	//地面
	plane_ = std::make_unique<Plane>();

	//時間カウント処理
	countTimer_ = std::make_unique<CountTimer>();

	//敵出現マネージャ生成
	enemySpawnManager_ = std::make_unique<EnemyManager>();
	enemySpawnManager_->SetPWorld(player_->world_);

	//カメラ関係クラス生成
	followCamera_ = std::make_unique<FollowCamera>();

	int texture;


	texture = TextureManager::LoadTex(white);
	sceneC_.reset(Sprite::Create(texture, { 1,1 }, { 1,1 }, { 1280,720 }));
	sceneC_->SetMaterialDataColor({ 0,0,0,1 });

	bgmGame_ = AudioManager::LoadSoundNum("game");



	ATKHitPerticle_ = std::make_unique<ParticleManager>();
	EmiterSphere* emit = ATKHitPerticle_->GetEmiterData();
	emit->speed = { 0.1f,1.5f };
}

GameScene::~GameScene() {
}

void GameScene::Initialize() {


	limitMinute = maxmilitMinute;

	player_->Initialize();

	//初期化
	camera_->Initialize();
	//各種設定
	camera_->SetTarget(&player_->world_);
	camera_->SetCameraDirection(-40.0f);
	player_->SetCamera(camera_);


	currentSceneXhangeSec_ = maxSceneChangeSec_;
	isSceneChange_ = false;
	sceneC_->SetColorAlpha(1);

	AudioManager::GetInstance()->StopAllSounds();


	ATKHitPerticle_->Initialize(TextureManager::LoadTex("resources/Texture/CG/circle.png"));
	ATKHitPerticle_->SetOnlyImpact(true);
	EmiterSphere* emit = ATKHitPerticle_->GetEmiterData();

	emit->color = { 0,0,1,1 };

	//初期シーンリクエスト
	sceneRequest_ = Other2ThisScene;
}



void GameScene::Update() {

	//デバッグウィンドウ表示
	DebugWindows();

	//エフェクトマネージャ追加
	ATKHitPerticle_->Update();

	//リクエストが存在しているなら処理
	if (sceneRequest_) {
		//リクエストの値を追加
		scene_ = sceneRequest_.value();
		//リクエストをクリア
		sceneRequest_ = std::nullopt;
		((this->*BehaviorInitialize[(int)scene_])());
	}

	//更新処理
	((this->*BehaviorUpdate[(int)scene_])());

	//モデル更新
	countTimer_->SpriteUpdate();
	player_->ObjectUpdate();
	enemySpawnManager_->ObjectUpdate();
	camera_->Update();
	
}

void GameScene::Draw() {

	//地面
	plane_->Draw();

	//敵の出現地点と敵描画
	enemySpawnManager_->Draw();

	//プレイヤー
	player_->Draw();

	//インスタンシングのモデルを全描画
	InstancingModelManager::GetInstance()->DrawAllModel();

	//パーティクル描画
	ATKHitPerticle_->Draw();
	player_->DrawParticle();

	//ポストプロセス処理
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kLightOutline, true);
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kVinetting, true);
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kHSV, true);

	//キル数取得
	int Count = enemySpawnManager_->GetKillCount();

	//キル数が200以上でさらに追加
	if (Count >= 200) {
		PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kRadialBlur, true);
	}

	//シーン変更前
	if (isSceneChange_) {
		//直前のシーンの画像読み込み
		PostEffectManager::GetInstance()->LoadScene2TexFlagActive();
	}
	else {//シーン変更前ではない場合
		//UIの描画
		UIDraw();

	}
	//シーン遷移時の画像描画
	sceneC_->Draw();
}

void GameScene::DebugWindows() {

#ifdef _DEBUG
	//ポストエフェクトのデバッグを描画
	PostEffectManager::GetInstance()->Debug();
#endif // _DEBUG

}

void GameScene::Collision() {

	//現在の敵番号
	int e1Num = 0;
	//敵関係の当たり判定
	for (auto& enemy : enemySpawnManager_->GetEnemies()) {
		//生存しているかチェック
		if (!enemy->GetDead()) {

			//プレイヤーの攻撃ヒット処理
			//プレイヤーが攻撃しているか否か
			if (player_->IsPlayerATK()) {

				//当たり判定チェック
				if (enemy->Collision(player_->GetCollider())) {
					//カメラシェイク処理
					followCamera_->SetShake();
					//エフェクトを発生
					ATKHitPerticle_->SpawnE(enemy->world_.GetWorldTranslate());
				}
			}
			//無条件でプレイヤーに押し出される
			enemy->OshiDashi(player_->GetCollider());

			//敵同士の当たり判定
			//2グループの敵番号
			int e2Num = 0;
			//配列でループ
			for (auto& enemy2 : enemySpawnManager_->GetEnemies()) {
				//死んでいない＆すでに当たっていない＆番号が同じではない場合
				if (!enemy2->GetDead() && !enemy2->isHit() && e1Num != e2Num) {
					//押し出しベクトルの計算
					Vector3 backV = enemy->OshiDashi(enemy2->GetCollider());
					//押し出し
					enemy2->AddTranslate(-backV);
				}
				//番号を加算
				e2Num++;
			}
		}

		//番号を加算
		e1Num++;
	}

}

void GameScene::SceneChange() {

	//カウントが終了時
	if (countTimer_->isCountEnd()) {
		//状態変更リクエスト設定
		sceneRequest_ = This2Other;
		//全ての音を止める
		AudioManager::GetInstance()->StopAllSounds();
	}

	//デバッグ用シーンチェンジ
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		sceneNo = (int)SCENE::GAMECLEAR;
	}
#endif // _DEBUG

	//ESCでゲーム強制終了
	if (input_->TriggerKey(DIK_ESCAPE)) {
		//ゲーム終了フラグをON
		leaveGame = true;
	}
}

void GameScene::UIDraw() {

	//時間カウント描画
	countTimer_->Draw();

	//プレイヤーUIの描画
	player_->DrawUI();
}

void GameScene::InitOther2This()
{
	//時間をリセット
	currentSceneXhangeSec_ = maxSceneChangeSec_;
}

void GameScene::InitThis()
{
	//BGMを再生
	AudioManager::PlaySoundData(bgmGame_, 0.08f);
}

void GameScene::InitThis2Other()
{
	//カウントリセット
	currentSceneXhangeSec_ = 0;
}

void GameScene::UpdateOther2This()
{

	//割合から透明度算出
	float alpha = float(currentSceneXhangeSec_ / maxSceneChangeSec_);

	//透明度を反映
	sceneC_->SetColorAlpha(alpha);

	//減算処理
	currentSceneXhangeSec_ -= (float)DeltaTimer::deltaTime_;

	//もし0以下になったら
	if (currentSceneXhangeSec_ <= 0.0f) {
		//透明度を0に設定
		sceneC_->SetColorAlpha(0);
		//状態リクエスト送信
		sceneRequest_ = ThisScene;
	}
}

void GameScene::UpdateThis()
{
	//時間経過処理更新
	countTimer_->GameUpdate();

	//プレイヤー更新
	player_->GameUpdate();

	//敵関係更新
	enemySpawnManager_->GameUpdate();

	//追従カメラ更新
	followCamera_->Update();

	//当たり判定処理
	Collision();

	//遷移処理
	SceneChange();
}

void GameScene::UpdateThis2Other()
{
	//透明度計算
	float alpha = float(currentSceneXhangeSec_ / maxSceneChangeSec_);

	//透明度を反映
	sceneC_->SetColorAlpha(alpha);

	currentSceneXhangeSec_ += (float)DeltaTimer::deltaTime_;

	//カウントが指定地を超えた場合
	if (currentSceneXhangeSec_ >= maxSceneChangeSec_) {
		//透明度を1に設定
		sceneC_->SetColorAlpha(1);
		//シーンを変更
		sceneNo = (int)SCENE::GAMECLEAR;
	}
}

//各処理の関数セット
//初期化
void (GameScene::* GameScene::BehaviorInitialize[])() {
	& GameScene::InitOther2This,
	& GameScene::InitThis,
	& GameScene::InitThis2Other
};

//更新
void (GameScene::* GameScene::BehaviorUpdate[])() {
	& GameScene::UpdateOther2This,
	& GameScene::UpdateThis,
	& GameScene::UpdateThis2Other
};


