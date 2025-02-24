#include"BrokenBody.h"
#include"RandomNum/RandomNum.h"
#include"TextureManager/TextureManager.h"



BrokenBody::BrokenBody()
{
	InstancingGameObject::Initialize("EBox");
	world_.scale_ = { 0.2f,0.2f,0.2f };
	effectDatas_.clear();

	int tex = TextureManager::LoadTex("resources/Models/Object/enemy.png");

	IMM_->SetTexture(tag_, tex);

	tree_.name_ = "砕け散るからだ";
	tree_.SetValue("吹き飛び速度", &upSPD_);
	tree_.SetValue("死亡時間", &maxDeadCount_);
	tree_.SetValue("跳ねたときの速度減少量", &bulletforce_);
	tree_.SetValue("サイズ", &scale_);
	
	//マテリアルツリーを追加
	tree_.SetTreeData(IMM_->CreateAndGetTree(tag_, "マテリアル"));
}

BrokenBody::~BrokenBody()
{
	effectDatas_.clear();
}

void BrokenBody::Update() {


	for (EffectData* eData : effectDatas_) {

		if (eData->deadCount++ >= maxDeadCount_) {
			eData->isdead_ = true;
		}
		else {

			//eData->world.scale_ = world_.scale_;
			eData->velocity_ += eData->accce_;
			eData->world.translate_ += eData->velocity_;

			if (eData->world.translate_.y <= 0) {
				eData->world.translate_.y = 0;
				eData->velocity_.y *= -1;
				eData->velocity_ *= eData->BulletForce;
			}


		}
	}


	effectDatas_.remove_if([](const EffectData* edata) {
		if (edata->isdead_) {
			return true;
		}
		return false;
		});
}

void BrokenBody::Draw() {

	for (auto& eData : effectDatas_) {
		eData->world.UpdateMatrix();

		IMM_->SetData(tag_, eData->world,color_);
	}

}


void BrokenBody::EffectOccurred(const EulerWorldTransform& world, int spawnNum) {

	for (int count = 0; count < spawnNum; ++count) {

		EffectData* edata = new EffectData();

		//ワールド
		edata->world = world;
		edata->velocity_ = {
			RandomNumber::Get(-upSPD_,upSPD_),
			upSPD_,
			RandomNumber::Get(-upSPD_,upSPD_)
		};
		edata->BulletForce = bulletforce_;
		edata->world.scale_ = { scale_,scale_,scale_ };
		effectDatas_.push_back(edata);
	}


}