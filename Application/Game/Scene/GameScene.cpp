#include"GameScene.h"

#include<imgui.h>

#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"RandomNum/RandomNum.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"

//#include"ColliderOBB/OBBCollider.h"
#include"DeltaTimer/DeltaTimer.h"

GameScene::GameScene() {

	//入力インスタンス取得
	input_ = Input::GetInstance();

	//カメラのインスタンス取得
	camera_ = Camera::GetInstance();
	//初期化
	camera_->Initialize();

	//プレイヤー生成
	player_ = std::make_unique<Player>();

	//プレイヤーをターゲットに指定
	camera_->SetTarget(&player_->world_);

	//地面
	plane_ = std::make_unique<Plane>();

	//時間カウント処理
	countTimer_ = std::make_unique<CountTimer>();

	//敵出現マネージャ生成
	enemyManager_ = std::make_unique<EnemyManager>(player_->world_);

	//カメラ関係クラス生成
	followCamera_ = std::make_unique<FollowCamera>();

	//スコアの保存マネジャ
	scoreSaveManager_ = std::make_unique<ScoreSaveManager>();

	//遷移クラス生成
	transition_ = std::make_unique<Transition>();

	//遷移画像読み込み

	bgmGame_ = AudioManager::LoadSoundNum("game");


	ATKHitPerticle_ = std::make_unique<ParticleManager>();
	ATKHitPerticle_->Initialize(TextureManager::LoadTex("resources/Texture/CG/circle.png"));
	ATKHitPerticle_->SetOnlyImpact(true);
	EmiterSphere* emit = ATKHitPerticle_->GetEmiterData();
	emit->speed = { 0.1f,1.5f };
	emit->color = { 0,0,1,1 };
}

GameScene::~GameScene() {
}

void GameScene::Initialize() {

	//プレイヤー初期化
	player_->Initialize();

	//出現マネージャの初期化
	//enemyManager_->Initialize();

	//全ての音を止める
	AudioManager::GetInstance()->StopAllSounds();

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
	//enemyManager_->ObjectUpdate();
	camera_->Update();

}

void GameScene::Draw() {

	//地面
	plane_->Draw();

	//敵の出現地点と敵描画
	enemyManager_->Draw();

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
	int Count = enemyManager_->GetKillCount();

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

	//遷移時の時のみ描画
	if (scene_ != ThisScene) {
		transition_->Draw();
	}

}

void GameScene::DebugWindows() {

#ifdef _DEBUG
	//ポストエフェクトのデバッグを描画
	PostEffectManager::GetInstance()->Debug();
#endif // _DEBUG

}

void GameScene::Collision() {

	//敵との当たり判定
	if (enemyManager_->Collision(player_->GetCollider(), player_->IsPlayerATK())) {
		//ヒットフラグが有効なので処理
		//カメラシェイク処理
		followCamera_->SetShake();
		//エフェクトを発生
		ATKHitPerticle_->SpawnE(player_->world_.GetWorldTranslate());
	}

}

void GameScene::SceneChange() {

	//カウントが終了時
	if (countTimer_->isCountEnd()) {
		//全ての音を止める
		AudioManager::GetInstance()->StopAllSounds();
		//状態変更リクエスト設定
		sceneRequest_ = This2Other;
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
	transition_->SetAndStartTransition(Transition::TransitionType::Black2Clear);
}

void GameScene::InitThis()
{
	//BGMを再生
	AudioManager::PlaySoundData(bgmGame_, 0.08f);
}

void GameScene::InitThis2Other()
{
	transition_->SetAndStartTransition(Transition::TransitionType::Clear2Black);
}

void GameScene::UpdateOther2This()
{
	//遷移が終了時
	if (transition_->Update()) {
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
	enemyManager_->Update();

	//追従カメラ更新
	followCamera_->Update();

	//当たり判定処理
	Collision();

	//遷移処理
	SceneChange();
}

void GameScene::UpdateThis2Other()
{
	if (transition_->Update()) {
		//スコアを保存する
		scoreSaveManager_->SaveScore(enemyManager_->GetKillCount(), 0);
		//シーンを変更
		sceneNo = (int)SCENE::GAMECLEAR;
	}
}

//各処理の関数セット
//初期化
void (GameScene::* GameScene::BehaviorInitialize[])() {
	&GameScene::InitOther2This,
		& GameScene::InitThis,
		& GameScene::InitThis2Other
};

//更新
void (GameScene::* GameScene::BehaviorUpdate[])() {
	&GameScene::UpdateOther2This,
		& GameScene::UpdateThis,
		& GameScene::UpdateThis2Other
};


