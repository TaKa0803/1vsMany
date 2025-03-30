#include "ClearScene.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"
#include"AudioManager/AudioManager.h"
#include"TextureManager/TextureManager.h"
#include"DeltaTimer/DeltaTimer.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

ClearScene::ClearScene()
{
	//入力クラス生成
	input_ = Input::GetInstance();

	//スコアセーブマネージャ生成
	std::unique_ptr<ScoreSaveManager>scoreSaveManager = std::make_unique<ScoreSaveManager>();
	scoreSaveManager->LoadScore();
	//データを読み込み
	scoreData_ = scoreSaveManager->GetData();

	//画像を読み込んでスプライト生成
	int texture;

	//黒背景生成
	texture = TextureManager::LoadTex("resources/Texture/AL/black.png");
	backScreen_.reset(Sprite::Create(texture, { 64,64 }, { 64,64 }, { 1280,720 }));

	//討伐数カウントクラス生成
	countKilledEnemies_ = std::make_unique<CountKilledEnemies>();

	//トランジションクラス生成
	transition_ = std::make_unique<Transition>();

	//BGMの番号取得
	bgmClear_ = AudioManager::LoadSoundNum("clear");


	//デバッグの値セット
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("ClearScene");
	gvg->SetTreeData(countKilledEnemies_->GetTree());
	gvg->SetTreeData(backScreen_->GetTree("黒背景"));

}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	//透明度を0に設定
	backScreen_->SetColorAlpha(0);

	//討伐数カウントクラス初期化
	countKilledEnemies_->Initialize(scoreData_.kill);

	//クリアBGMの再生
	AudioManager::PlaySoundData(bgmClear_, 0.08f);
}

void ClearScene::Update()
{
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
}

void ClearScene::Draw()
{
	//黒背景描画
	backScreen_->Draw();

	//数字描画
	countKilledEnemies_->Draw();

	//遷移時の時のみ描画
	if (scene_ != ThisScene) {
		transition_->Draw();
	}
}


void ClearScene::InitOther2This()
{
	//遷移の初期化
	transition_->SetAndStartTransition(Transition::TransitionType::Black2Clear);
}

void ClearScene::InitThis()
{
}

void ClearScene::InitThis2Other()
{
	//遷移の初期化
	transition_->SetAndStartTransition(Transition::TransitionType::Clear2Black);
}

void ClearScene::UpdateOther2This()
{
	//遷移が終了時
	if (transition_->Update()) {
		//
		sceneRequest_ = ThisScene;
	}
}

void ClearScene::UpdateThis()
{
	//キルカウント処理
	countKilledEnemies_->Update();

	//カウント処理が終了かつシーン遷移フラグが立っていない場合
	if (countKilledEnemies_->CheckComplete()) {
		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerButton(kButtonB)) {
			//遷移処理に移行
			sceneRequest_ = This2Other;
		}
	}
}

void ClearScene::UpdateThis2Other()
{
	if (transition_->Update()) {
		//シーンを変更
		sceneNo = (int)SCENE::TITLE;
	}
}

//各処理の関数セット
//初期化
void (ClearScene::* ClearScene::BehaviorInitialize[])() {
	&ClearScene::InitOther2This,
	& ClearScene::InitThis,
	& ClearScene::InitThis2Other
};

//更新
void (ClearScene::* ClearScene::BehaviorUpdate[])() {
	&ClearScene::UpdateOther2This,
	& ClearScene::UpdateThis,
	& ClearScene::UpdateThis2Other
};