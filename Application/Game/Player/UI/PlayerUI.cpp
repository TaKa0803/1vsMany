#include "PlayerUI.h"
#include"TextureManager/TextureManager.h"

PlayerUI::PlayerUI(const int& comboCount)
{
	comboCount_ = &comboCount;

	int texture;
	skillTex_ = TextureManager::LoadTex("resources/Texture/AL/skill.png");
	
	baseSkillSp_.reset(Sprite::Create(skillTex_, { 90,90 }, { 90,90 }, { 90,90 }, { 1000,600 }));
	skillSp_.reset(Sprite::Create(skillTex_, { 180,90 }, { 90,90 }, { 90,90 }, { 1000,600 }));

	texture = TextureManager::LoadTex("resources/Texture/AL/B.png");
	BButton_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 64,64 }, { 1045, 650 }));

	//スプライト生成
	preSkillSprite_.reset(Sprite::Create(TextureManager::white_, { 180,90 }, { 90,90 }, { 90,90 }, { 1000,600 }));


	punchTex_ = TextureManager::LoadTex("resources/Texture/AL/punch.png");
	//punchSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 90,90 }, { 1070,650 }));

	kickTex_ = TextureManager::LoadTex("resources/Texture/AL/kick.png");
	//kickSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 90,90 }, { 1070,650 }));


	guruguruTex_ = TextureManager::LoadTex("resources/Texture/AL/ult.png");
	//ultSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 90,90 }, { 1070,650 }));


}


void PlayerUI::Init()
{
	baseSkillSp_->SetPosition(ATKpos);
	baseSkillSp_->SetScale(ATKscale);
	skillSp_->SetPosition(ATKpos);
	skillSp_->SetScale(ATKscale);
	skillSp_->SetUVTranslate({ 0.0f,0.0f });
	preSkillSprite_->SetPosition(waitATKpos);
	preSkillSprite_->SetScale(waitATKscale);
	preSkillSprite_->SetUVTranslate({ 0.5f,0 });
	BButton_->SetPosition({ 1155,635 });
}

void PlayerUI::Update()
{
	//各コンボ状態での画像セット

	//何もしていない場合
	if (*comboCount_ == 0) {
		//１コンボ目の画像をセット
		//skillSp_->SetTexture(skillTex_);
		preSkillSprite_->SetTexture(punchTex_);
	}
	else if (*comboCount_ == 1) {
		skillSp_->SetTexture(punchTex_);
		preSkillSprite_->SetTexture(kickTex_);
	}
	else if (*comboCount_ == 2) {
		skillSp_->SetTexture(kickTex_);
		preSkillSprite_->SetTexture(guruguruTex_);
	}
	else if (*comboCount_ == 3) {
		skillSp_->SetTexture(guruguruTex_);
		//preSkillSprite_->SetTexture(skillTex_);
	}

}

void PlayerUI::Draw()
{

	//何もしていない場合
	if (*comboCount_ == 0) {
		//空スキルUI描画
		baseSkillSp_->Draw();
	}
	else {
		//現在の攻撃スキルUI描画
		skillSp_->Draw();
	}

	//最後のコンボ以外の場合
	if (*comboCount_ != 3) {
		//次の攻撃スキルUI描画
		preSkillSprite_->Draw();
	}
	BButton_->Draw();
}
