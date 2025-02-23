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
		const int maxDeadCount = 180;
		int deadCount = 0;
		bool isdead_ = false;

	};

	float upSPD_ = 1.0f;


	std::list<EffectData*>effectDatas_;

};