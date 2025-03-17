#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

#include<list>

class BrokenBody :public InstancingGameObject {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BrokenBody();
	~BrokenBody()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクト生成
	/// </summary>
	/// <param name="world">出現中心座標</param>
	/// <param name="spawnNum">出現数</param>
	void EffectOccurred(const EulerWorldTransform& world);

private://**プライベート変数**//

	//エフェクトデータ
	struct EffectData {
		EulerWorldTransform world;
		Vector3 velocity;
		float BulletForce = 0.4f;
		float currentSec = 0;
		bool isdead = false;

	};

	//エフェクトデータ群
	std::list<std::unique_ptr<EffectData>>effectDatas_;

private://**パラメータ変数**//

	//生成数
	int spawnNum_ = 10;
	//加速度
	Vector3 acceleration_ = {0,-0.1f,0};
	//吹き飛び速度
	float upSpeed_ = 1.0f;
	//志望時間
	float maxDeadSec_ = 3.0f;
	//跳ねたときの速度減少量
	float bulletforce_ = 0.4f;
	//サイズ
	float scale_ = 1.0f;



};