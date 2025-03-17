#include"BrokenBody.h"
#include"DeltaTimer/DeltaTimer.h"
#include"RandomNum/RandomNum.h"
#include"TextureManager/TextureManager.h"



BrokenBody::BrokenBody()
{
	//タグミスがないかチェック
	InstancingGameObject::Initialize("EBox");

	//画像取得
	int tex = TextureManager::LoadTex("resources/Models/Object/enemy.png");
	//タグモデルに画像をセット
	IMM_->SetTexture(tag_, tex);

	tree_.name_ = "砕け散るからだ";
	tree_.SetValue("生成数", &spawnNum_);
	tree_.SetValue("加速度", &acceleration_);
	tree_.SetValue("吹き飛び速度", &upSpeed_);
	tree_.SetValue("死亡時間", &maxDeadSec_);
	tree_.SetValue("跳ねたときの速度減少量", &bulletforce_);
	tree_.SetValue("サイズ", &scale_);
	
	//マテリアルツリーを追加
	tree_.SetTreeData(IMM_->CreateAndGetTree(tag_, "マテリアル"));
}

void BrokenBody::Update() {

	//エフェクトの更新
	for (auto& eData : effectDatas_) {
		//時間の経過
		eData->currentSec += (float)DeltaTimer::deltaTime_;

		//時間が規定以上になったとき
		if (eData->currentSec >= maxDeadSec_) {
			//死亡フラグを有効にする
			eData->isdead = true;
		}
		else {//規定時間内の場合

			//加速度を速度に加算
			eData->velocity += acceleration_;
			//速度を座標に加算
			eData->world.translate_ += eData->velocity;
			
			//もし0以下の場合
			if (eData->world.translate_.y <= 0) {
				//0に設定
				eData->world.translate_.y = 0;
				//速度を反転する
				eData->velocity.y *= -1;
				//弾性倍率をかける
				eData->velocity *= eData->BulletForce;
			}
		}
	}

	//エフェクトの削除処理
	effectDatas_.remove_if([](auto& edata) {
		//死亡している場合削除
		if (edata->isdead) {
			return true;
		}
		return false;
		});
}

void BrokenBody::Draw() {
	//エフェクトデータ更新して描画設定
	for (auto& eData : effectDatas_) {
		//行列更新
		eData->world.UpdateMatrix();
		//座標データを送る
		IMM_->SetData(tag_, eData->world);
	}
}


void BrokenBody::EffectOccurred(const EulerWorldTransform& world) {

	//数分生成
	for (int count = 0; count < spawnNum_; ++count) {

		//データ生成
		std::unique_ptr<EffectData> edata =std::make_unique<EffectData>();

		//ワールド座標を合わせる
		edata->world = world;
		//吹き飛ぶ方向をランダムに設定
		edata->velocity = {
			RandomNumber::Get(-upSpeed_,upSpeed_),
			upSpeed_,
			RandomNumber::Get(-upSpeed_,upSpeed_)
		};
		//弾性力を設定
		edata->BulletForce = bulletforce_;
		//サイズ設定
		edata->world.scale_ = { scale_,scale_,scale_ };
		//データを追加
		effectDatas_.push_back(std::move(edata));
	}
}