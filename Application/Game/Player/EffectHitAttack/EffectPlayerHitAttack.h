#pragma once
#include"ParticleData/ParticleManager.h"

//攻撃Hitエフェクトクラス
class EffectPlayerHitAttack
{
public://**パブリック関数**//

	EffectPlayerHitAttack();
	~EffectPlayerHitAttack() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private://**プライベート関数**//

	//攻撃エフェクトクラス
	std::unique_ptr<ParticleManager>AttackHitPerticle_;

	//エミットデータ
	EmiterSphere* emit_;
};