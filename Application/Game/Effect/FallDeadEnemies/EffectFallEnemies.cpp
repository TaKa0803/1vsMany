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
	gvg->SetValue("最小最大移動速度", &speeds_);
	gvg->SetValue("最小最大加速度", &fallSpeeds_);
	gvg->SetValue("回転速度", &rotateSpeed_);
	gvg->SetValue("最大死亡時間", &maxDeadSec_);
	gvg->SetValue("最小最大反発力", &repulsion_);
	gvg->SetValue("反発高度", &repulsionHeight_);
	gvg->SetValue("生成間隔", &maxSpawnSec_);
	gvg->SetValue("同時生成数",&spawnNum_);
	gvg->SetTreeData(InstancingModelManager::GetInstance()->CreateAndGetTree(modelName_,"敵モデル"));
	gvg->SetMonitorValue("敵の数", &count_);
	gvg->SetMonitorValue("生成数", &spawnCount_);

	gvg->SetMonitorValue("敵の全削除", &clearEnemy_);

}

void EffectFallEnemies::Initialize(const int count)
{
	//敵の数を設定
	count_ = count;
}

void EffectFallEnemies::Update()
{
	if (clearEnemy_) {
		//全削除フラグが立っていたら
		enemies_.clear();
		clearEnemy_ = false;
	}

	//デバッグ用にサイズを設定
	spawnCount_ = (int)enemies_.size();

	//生成分ループ
	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	//不要なものを削除
	enemies_.remove_if([](auto& enemy) {
		//死亡フラグが立っていたら
		if (enemy->IsDead()) {
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
		enemy->Draw();
	}
}

void EffectFallEnemies::EmitEnemies()
{
	//数が多い場合処理を行わない
	if (enemies_.size() >= count_) {
		return;
	}


	spawnSec_ += (float)DeltaTimer::deltaTime_;

	if (spawnSec_ >= maxSpawnSec_) {
		spawnSec_ = 0;

		float pi = (float)std::numbers::pi;

	//指定数分生成
	for (int i = 0; i < spawnNum_; i++) {
		//数が多い場合処理を行わない
		if (enemies_.size() >= count_) {
			return;
		}

		//敵をパラメータ生成
		FallEnemy::Parameters parameters;
		EulerWorldTransform world;

		//ランダムなYがゼロの向きベクトル作成
		Vector3 range = { RandomNumber::Get(-1.0f,1.0f),0,RandomNumber::Get(-1.0f,1.0f) };
		range.SetNormalize();

		//敵の座標を設定
		world.translate_ = spawnCenterPosition_ + range * RandomNumber::Get(0, spawnRange_);
		//敵の回転量を設定
		world.rotate_= { RandomNumber::Get(-pi, pi),RandomNumber::Get(-pi, pi),RandomNumber::Get(-pi, pi) };
		//敵の移動速度を設定
		parameters.velocity *= RandomNumber::Get(speeds_.x, speeds_.y);
		//敵の回転速度を設定
		parameters.rotate = { RandomNumber::Get(-rotateSpeed_, rotateSpeed_),RandomNumber::Get(-rotateSpeed_, rotateSpeed_),RandomNumber::Get(-rotateSpeed_, rotateSpeed_) };
		//死亡時間を設定
		parameters.deadSec = (maxDeadSec_);
		//最小最大反発力を設定
		parameters.repulsion = RandomNumber::Get(fallSpeeds_.x, fallSpeeds_.y);
		//反発高度を設定
		parameters.repulsionHeight = repulsionHeight_;

		//新しいパラメータを渡した物を生成
		std::unique_ptr<FallEnemy> enemy = std::make_unique<FallEnemy>(world,parameters);
		enemy->Initialize(modelName_);;
		//データを追加
		enemies_.push_back(std::move(enemy));
	}
}
}
