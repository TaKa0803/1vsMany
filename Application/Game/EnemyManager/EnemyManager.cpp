#include "EnemyManager.h"
#include"TextureManager/TextureManager.h"
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

	//数字の連番画像を読み込んでスプライト生成
	int texture = TextureManager::LoadTex("resources/Texture/AL/number64x90.png");
	//番号スプライト生成
	num1_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));
	num10_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));
	num100_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));

	textBaseWorld_.translate_ = { 640,360 };
	num1_->SetParent(textBaseWorld_);
	num10_->SetParent(textBaseWorld_);
	num100_->SetParent(textBaseWorld_);



	//デバッグ用に値を追加
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("AboutEnemy");
	gvg->SetTreeData(spawnManager_->GetTree());
	gvg->SetTreeData(parameterManager_->GetTree());

	//体が壊れる演出のツリー追加
	gvg->SetTreeData(brokenBody_->tree_);

	//モニター値を追加
	gvg->SetMonitorValue("キル数", &killCount_);
	gvg->SetMonitorValue("敵の数", &enemyCount_);
	gvg->SetMonitorValue("敵を全て削除", &clearEnemy_);

	GvariTree countUI;
	countUI.name_ = "キル数カウント関係";
	countUI.SetTreeData(textBaseWorld_.GetDebugTree("テキスト全体ワールド"));
	countUI.SetTreeData(num1_->GetTree("1の位"));
	countUI.SetTreeData(num10_->GetTree("10の位"));
	countUI.SetTreeData(num100_->GetTree("100の位"));

	gvg->SetTreeData(countUI);
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
	if (clearEnemy_) {
		enemies_.clear();
	}
#endif // _DEBUG


	//敵の生成処理
	SpawnEnemy();

	//敵の更新と削除処理
	UpdateEnemies();

	//UIのUV更新
	SetTxetureUV();

	textBaseWorld_.UpdateMatrix();

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

void EnemyManager::DrawUI()
{
	//数字描画
	num1_->Draw();
	num10_->Draw();
	num100_->Draw();
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

void EnemyManager::SetTxetureUV()
{
	//1の位の値を求める
	int num1 = killCount_ % 10;
	//一桁目決定
	num1_->SetUVTranslate({ ((float)num1 / 10.0f) - 0.1f ,0 });

	//1の位の値を引いた値を10で割る
	int count2 = (int)(killCount_ - num1) / 10;
	//10の位の値を求める
	int num2 = count2 % 10;
	//カウントが10以上の場合
	if (killCount_ >= 10) {
		//10の位決定
		num10_->SetUVTranslate({ ((float)num2 / 10.0f) - 0.1f ,0 });

		//カウントが100以上の場合
		if (killCount_ >= 100) {
			//1,10の位の値を引いた値を100で割る
			int count3 = (int)(killCount_ - (num1 + num2 * 10)) / 100;
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
