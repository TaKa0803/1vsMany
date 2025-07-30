#include "EffectPlayerHitAttack.h"
#include"TextureManager/TextureManager.h"

EffectPlayerHitAttack::EffectPlayerHitAttack()
{
	//攻撃ヒットエフェクトの生成と初期化
	AttackHitPerticle_ = std::make_unique<ParticleManager>();
	AttackHitPerticle_->Initialize(TextureManager::LoadTex("resources/Texture/CG/circle.png"));
	AttackHitPerticle_->SetOnlyImpact(true);
	emit_ = AttackHitPerticle_->GetEmiterData();
	emit_->speed = { 0.1f,1.5f };
	emit_->color = { 0,0,1,1 };

	//エフェクトマネージャ追加
	//AttackHitPerticle_->Update();

	//パーティクル描画
	//AttackHitPerticle_->Draw();
}
