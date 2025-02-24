#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

#include<list>

class BrokenBody :public InstancingGameObject {

public:

	BrokenBody();
	~BrokenBody();


	void Update();

	void Draw();

	/// <summary>
	/// エフェクト生成
	/// </summary>
	/// <param name="world">出現中心座標</param>
	/// <param name="spawnNum">出現数</param>
	void EffectOccurred(const EulerWorldTransform& world, int spawnNum);


	struct EffectData {
		EulerWorldTransform world;
		Vector3 velocity_;
		Vector3 accce_ = { 0,-0.1f,0 };
		float BulletForce = 0.4f;
		int deadCount = 0;
		bool isdead_ = false;

	};

	//吹き飛び速度
	float upSPD_ = 1.0f;
	//志望時間
	int maxDeadCount_ = 180;
	//跳ねたときの速度減少量
	float bulletforce_ = 0.4f;

	float scale_ = 1.0f;


	std::list<EffectData*>effectDatas_;

};