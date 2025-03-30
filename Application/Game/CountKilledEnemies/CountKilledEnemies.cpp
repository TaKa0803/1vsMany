#include "CountKilledEnemies.h"
#include"TextureManager/TextureManager.h"

CountKilledEnemies::CountKilledEnemies()
{
	//画像を読み込んでスプライト生成
	int texture;

	//リザルトテキスト生成
	texture = TextureManager::LoadTex("resources/Texture/AL/result.png");
	resultText_.reset(Sprite::Create(texture, { 320,90 }, { 320,90 }, { 830,290 }));

	texture = TextureManager::LoadTex("resources/Texture/AL/number64x90.png");

	//番号スプライト生成
	num1_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));
	num10_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));
	num100_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));


	tree_.name_ = "敵カウント";
	tree_.SetTreeData(resultText_->GetTree("文字"));
	tree_.SetTreeData(num1_->GetTree("1の位"));
	tree_.SetTreeData(num10_->GetTree("10の位"));
	tree_.SetTreeData(num100_->GetTree("100の位"));


}

void CountKilledEnemies::Initialize(int killNum)
{
	//討伐数を設定
	killNum_ = killNum;

	//カウント終了フラグをリセット
	isSerchComplete_ = false;
}

void CountKilledEnemies::Update()
{
	//カウント未終了の場合
	if (!isSerchComplete_) {

		//カウント増加
		currentCountEnemy_ += frameCountEnemy_;

		//カウントをtrueにする（最大数＞カウント済の場合falseにする処理がある
		isSerchComplete_ = true;

		//このフレームでのカウント数変数
		int count = 0;
		//カウント処理
		for (int i = 0; i < killNum_; i++) {
			//カウント量増加
			count++;
			//このフレームでの最大数を超えた場合
			if (count >= currentCountEnemy_) {
				//フラグをfalseに設定
				isSerchComplete_ = false;
				//ループを抜ける
				break;
			}
		}

		//数字スプライトのUV座標を変更
		SetUV(count);
	}
}

void CountKilledEnemies::Draw()
{
	//テキスト描画
	resultText_->Draw();

	//数字描画
	num1_->Draw();
	num10_->Draw();
	num100_->Draw();


}

void CountKilledEnemies::SetUV(int count)
{
	//1の位の値を求める
	int num1 = count % 10;
	//一桁目決定
	num1_->SetUVTranslate({ ((float)num1 / 10.0f) - 0.1f ,0 });

	//1の位の値を引いた値を10で割る
	int count2 = (int)(count - num1) / 10;
	//10の位の値を求める
	int num2 = count2 % 10;
	//カウントが10以上の場合
	if (count >= 10) {
		//10の位決定
		num10_->SetUVTranslate({ ((float)num2 / 10.0f) - 0.1f ,0 });

		//カウントが100以上の場合
		if (count >= 100) {
			//1,10の位の値を引いた値を100で割る
			int count3 = (int)(count - (num1 + num2 * 10)) / 100;
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
