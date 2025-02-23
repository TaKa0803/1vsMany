#include "ClearScene.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"
#include"AudioManager/AudioManager.h"
#include"TextureManager/TextureManager.h"
#include"DeltaTimer/DeltaTimer.h"

ClearScene::ClearScene()
{
	//入力クラス生成
	input_ = Input::GetInstance();

	//スコアセーブマネージャ生成
	std::unique_ptr<ScoreSaveManager>scoreSaveManager_ = std::make_unique<ScoreSaveManager>();

	//データを読み込み
	scoreData_ = scoreSaveManager_->GetData();

	//画像を読み込んでスプライト生成
	int texture;
	texture = TextureManager::LoadTex("resources/Texture/AL/number64x90.png");

	//番号スプライト生成
	num1_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));
	num10_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));
	num100_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));

	//黒背景生成
	texture = TextureManager::LoadTex("resources/Texture/AL/black.png");
	backScreen_.reset(Sprite::Create(texture, { 64,64 }, { 64,64 }, { 1280,720 }));

	//リザルトテキスト生成
	texture = TextureManager::LoadTex("resources/Texture/AL/result.png");
	resultText_.reset(Sprite::Create(texture, { 320,90 }, { 320,90 }, { 830,290 }));

	//遷移時のスプライト生成
	texture = TextureManager::white_;
	sceneC_.reset(Sprite::Create(texture, { 1,1 }, { 1,1 }, { 1280,720 }));
	sceneC_->SetMaterialDataColor({ 0,0,0,1 });

	//BGMの番号取得
	bgmClear_ = AudioManager::LoadSoundNum("clear");
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	//透明度を0に設定
	backScreen_->SetColorAlpha(0);
	resultText_->SetColorAlpha(0);
	//透明度をセット
	sceneC_->SetColorAlpha(0);

	//値リセット
	alphaNum_ = 0;
	currentCountEnemy_ = 0;

	//座標設定
	num1_->SetPosition({ 368,285 });
	num1_->SetScale({ 160,160 });

	num10_->SetPosition({ 238,285 });
	num10_->SetScale({ 160,160 });

	num100_->SetPosition({ 114,285 });
	num100_->SetScale({ 160,160 });

	resultText_->SetPosition({ 830,290 });

	//カウント終了フラグをリセット
	serchComplete_ = false;

	//クリアBGMの再生
	AudioManager::PlaySoundData(bgmClear_, 0.08f);
}

void ClearScene::Update()
{
	//キルカウント処理
	CountKill();

	//フェードイン処理
	FadeIn();

	//暗転処理
	BlackOut();

	//シーン変更処理
	if (serchComplete_) {
		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerButton(kButtonB)) {
			
			alphaNum_ = 0;
			isSceneChange_ = true;
		}
	}

}

void ClearScene::Draw()
{
	//ポストプロセス
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kDepthBasedOutline, true);
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kGaussianFilter, true);
	PostEffectManager::GetInstance()->PostEffectDraw(PostEffectManager::kGrayScale, true);
	
	//黒背景描画
	backScreen_->Draw();
	//テキスト描画
	resultText_->Draw();

	//数字描画
	num1_->Draw();
	num10_->Draw();
	num100_->Draw();

	//シーン画像描画
	sceneC_->Draw();
}

void ClearScene::CountKill()
{
	//カウント未終了の場合
	if (!serchComplete_) {

		//カウント増加
		currentCountEnemy_ += frameCountEnemy_;

		//カウントをtrueにする（最大数＞カウント済の場合falseにする処理がある
		serchComplete_ = true;
		
		//このフレームでのカウント数変数
		int Count = 0;
		//カウント処理
		for (int i = 0; i < scoreData_.kill; i++) {
			//カウント量増加
			Count++;
			//このフレームでの最大数を超えた場合
			if (Count >= currentCountEnemy_) {
				//フラグをfalseに設定
				serchComplete_ = false;
				//ループを抜ける
				break;
			}
		}

		//スプライトのUV座標変更処理

		//1の位の値を求める
		int num1 = Count % 10;
		//一桁目決定
		num1_->SetUVTranslate({ ((float)num1 / 10.0f) - 0.1f ,0 });

		//1の位の値を引いた値を10で割る
		int count2 = (int)(Count - num1) / 10;
		//10の位の値を求める
		int num2 = count2 % 10;
		//カウントが10以上の場合
		if (Count >= 10) {
			//10の位決定
			num10_->SetUVTranslate({ ((float)num2 / 10.0f) - 0.1f ,0 });

			//カウントが100以上の場合
			if (Count >= 100) {
				//1,10の位の値を引いた値を100で割る
				int count3 = (int)(Count - (num1 + num2 * 10)) / 100;
				//100の位の値を求める
				int num3 = count3 % 10;
				//100の位の値決定
				num100_->SetUVTranslate({ ((float)num3 / 10.0f) - 0.1f ,0 });
			}
			else {
				//0に設定
				num100_->SetUVTranslate({ 0.9f, 0 });
			}
		}
		else {
			//10,100の位の値を0に設定
			num100_->SetUVTranslate({ 0.9f, 0 });
			num10_->SetUVTranslate({ 0.9f, 0 });
		}
	}
}

void ClearScene::FadeIn()
{
	//背景の透明処理
	//シーン変更処理を行っていない場合のみの処理
	if (isSceneChange_) {
		//カウントが未達の場合
		if (alphaNum_ < addAlphaSec_) {

			//値を加算
			alphaNum_ += (float)DeltaTimer::deltaTime_;

			//割合から透明度を作成
			float screenAlpha = alphaNum_ / addAlphaSec_;

			//超えていたらセット
			if (screenAlpha >= screenmaxAlphaNum_) {
				screenAlpha = screenmaxAlphaNum_;
			}

			//値をセット
			backScreen_->SetColorAlpha(screenAlpha);

			//文字テキストの透明度設定
			float resultAlpha = alphaNum_ / addAlphaSec_;
			resultText_->SetColorAlpha(resultAlpha);

		}
		else {
			//値をセット
			backScreen_->SetColorAlpha(screenmaxAlphaNum_);

			//透明度を1に設定
			resultText_->SetColorAlpha(1.0f);
		}
	}
}

void ClearScene::BlackOut()
{
	//遷移フラグがONの時処理
	if (isSceneChange_) {
		//カウント達成で遷移
		if (alphaNum_ >= blackoutSec_) {
			//透明度をセット
			sceneC_->SetColorAlpha(1);
			//タイトルシーンに移動
			sceneNo = (int)SCENE::TITLE;
			//音をすべて止める
			AudioManager::GetInstance()->StopAllSounds();
		}
		else {
			//割合計算
			float alpha = alphaNum_ / blackoutSec_;
			//透明度をセット
			sceneC_->SetColorAlpha(alpha);
			//加算
			alphaNum_ += (float)DeltaTimer::deltaTime_;
		}
	}

}
