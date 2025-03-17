#include "EnemyManager.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"AudioManager/AudioManager.h"

EnemyManager::EnemyManager(const EulerWorldTransform& playerWorld)
{
	//敵出現管理クラス生成
	spawnManager_ = std::make_unique<EnemySpawnManager>(playerWorld);
	//敵のパラメータ管理クラス生成
	parameterManager_ = std::make_unique<EnemyParameterManager>();
	
	//エフェクトクラス生成
	brokenBody_ = std::make_unique<BrokenBody>();

	//死亡音の配列番号取得
	breakSound_ = AudioManager::LoadSoundNum("break");

	//デバッグ用に値を追加
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("AboutEnemy");
	gvg->SetTreeData(spawnManager_->GetTree());
	gvg->SetTreeData(parameterManager_->GetTree());

	//体が壊れる演出のツリー追加
	gvg->SetTreeData(brokenBody_->tree_);

	//モニター値を追加
	gvg->SetMonitorValue("キル数", &killCount_);
	gvg->SetMonitorValue("敵の数", &enemyCount_);
}

void EnemyManager::Init()
{
	//アニメーション関係のパラメータをセット
	parameterManager_->SetAnimationParameter();
}

void EnemyManager::Update()
{

#ifdef _DEBUG
	//デバッグ用値にデータを渡す
	enemyCount_ = (int)enemies_.size();
	//アニメーション関係のパラメータをセット
	parameterManager_->SetAnimationParameter();
#endif // _DEBUG


	//敵の生成処理
	SpawnEnemy();

	//敵の更新と削除処理
	UpdateEnemies();

	//エフェクトの更新
	brokenBody_->Update();
}

void EnemyManager::Draw()
{
	//エフェクトの描画
	brokenBody_->Draw();

	//敵
	for (auto& enemy : enemies_) {
		if (!enemy->GetDead()) {
			enemy->Draw();
		}
	}

}

bool EnemyManager::Collision(SphereCollider* collider,bool isPlayerATK)
{
	//攻撃ヒットフラグ
	bool isHit = false;

	//現在の敵番号
	int e1Num = 0;
	//敵関係の当たり判定
	for (auto& enemy : enemies_) {
		//生存しているかチェック
		if (!enemy->GetDead()) {

			//プレイヤーの攻撃ヒット処理
			//プレイヤーが攻撃しているか否か
			if (isPlayerATK) {

				//当たり判定チェック
				if (enemy->Collision(collider)) {
					//ヒットフラグを有効
					isHit = true;				
				}
			}
			//無条件でプレイヤーに押し出される
			enemy->OshiDashi(collider);

			//敵同士の当たり判定
			//2グループの敵番号
			int e2Num = 0;
			//配列でループ
			for (auto& enemy2 : enemies_) {
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

	return isHit;
}

void EnemyManager::SpawnEnemy()
{
	//生成処理の更新
	spawnManager_->Update();

	//生成したリストを取得
	std::list<std::unique_ptr<Enemy>>newEnemies = spawnManager_->SpawnEnemy(enemyCount_,parameterManager_->GetParameters());
	//生成データがある場合に処理
	if (newEnemies.size() != 0) {
		//後ろにデータ追加
		enemies_.insert(enemies_.end(),
			std::make_move_iterator(newEnemies.begin()),
			std::make_move_iterator(newEnemies.end())
		);
	}
}

void EnemyManager::UpdateEnemies()
{
	//敵の更新と削除処理
	enemies_.remove_if([&](std::unique_ptr<Enemy>& enemy) {

		//死亡フラグ取得
		if (!enemy->GetDead()) {
			//更新
			enemy->Update();
			//削除しない
			return false;
		}
		else {
			//エフェクトを生成
			brokenBody_->EffectOccurred(enemy->world_);

			//キルカウント増加
			killCount_++;

			//音発生
			AudioManager::PlaySoundData(breakSound_, 0.2f);

			//リストから削除
			return true;
		}

		});
}
