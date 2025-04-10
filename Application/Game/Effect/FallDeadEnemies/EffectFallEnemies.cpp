#include "EffectFallEnemies.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"RandomNum/RandomNum.h"
#include"DeltaTimer/DeltaTimer.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include<numbers>

EffectFallEnemies::EffectFallEnemies()
{
	//モデルタグの存在をチェック
	InstancingModelManager::GetInstance()->SerchTag(modelName_);

	//デバッグ用にパラメータ設定
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("EffectFallEnemies");
	gvg->SetValue("出現中心点", &spawnCenterPosition_);
	gvg->SetValue("出現範囲", &spawnRange_);
	gvg->SetValue("移動速度", &speeds_);
	gvg->SetValue("回転速度", &rotateSpeed_);
	gvg->SetValue("最大死亡時間", &maxDeadSec_);
	gvg->SetValue("生成間隔", &maxSpawnSec_);
	gvg->SetValue("同時生成数",&spawnNum_);
	gvg->SetTreeData(InstancingModelManager::GetInstance()->CreateAndGetTree(modelName_,"敵モデル"));
	gvg->SetMonitorValue("敵の数", &count_);
	gvg->SetMonitorValue("生成数", &spawnCount_);

}

void EffectFallEnemies::Initialize(const int count)
{
	//敵の数を設定
	count_ = count;
}

void EffectFallEnemies::Update()
{
	//デバッグ用にサイズを設定
	spawnCount_ = (int)enemies_.size();

	//生成分ループ
	for (auto& enemy : enemies_) {
		//死亡時間カウント
		enemy->deadSec += (float)DeltaTimer::deltaTime_;

		//時間が規定を超えたら
		if (enemy->deadSec >= maxDeadSec_) {
			//死亡フラグを立てる
			enemy->isDead = true;
		}

		//位置を更新
		enemy->transform.translate_ += enemy->velocity;
		//回転を更新
		enemy->transform.rotate_ += enemy->rotate;
		//行列を更新
		enemy->transform.UpdateMatrix();
	}

	//不要なものを削除
	enemies_.remove_if([](auto& enemy) {
		//死亡フラグが立っていたら
		if (enemy->isDead) {
			return true;
		}
		else {
		return false;
	}
		});

}

void EffectFallEnemies::Draw()
{
	//座標所持分ループ
	for (auto& enemy : enemies_) {
		//モデルを描画
		InstancingModelManager::GetInstance()->SetData(modelName_, enemy->transform);
	}
}

void EffectFallEnemies::EmitEnemies()
{
	spawnSec_ += (float)DeltaTimer::deltaTime_;

	if (spawnSec_ >= maxSpawnSec_) {
		spawnSec_ = 0;

		float pi = (float)std::numbers::pi;

	//指定数分生成
	for (int i = 0; i < spawnNum_; i++) {
		//敵をパラメータ生成
		std::unique_ptr<Parameters> enemy = std::make_unique<Parameters>();

		//ランダムなYがゼロの向きベクトル作成
		Vector3 range = { RandomNumber::Get(-1.0f,1.0f),0,RandomNumber::Get(-1.0f,1.0f) };
		range.SetNormalize();

		//敵の座標を設定
		enemy->transform.translate_ = spawnCenterPosition_ + range * RandomNumber::Get(0, spawnRange_);
		//敵の回転量を設定
		enemy->transform.rotate_= { RandomNumber::Get(-pi, pi),RandomNumber::Get(-pi, pi),RandomNumber::Get(-pi, pi) };
		//敵の移動速度を設定
		enemy->velocity *= RandomNumber::Get(speeds_.x, speeds_.y);
		//敵の回転速度を設定
		enemy->rotate = { RandomNumber::Get(-rotateSpeed_, rotateSpeed_),RandomNumber::Get(-rotateSpeed_, rotateSpeed_),RandomNumber::Get(-rotateSpeed_, rotateSpeed_) };



		//データを追加
		enemies_.push_back(std::move(enemy));
	}
}
}
