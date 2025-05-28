#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

//画面内でジャンプする敵
class JumpEnemy: public InstancingGameObject{
public:

	JumpEnemy();
	~JumpEnemy()=default;

	//更新
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ワールドデータ取得
	/// </summary>
	/// <returns></returns>
	EulerWorldTransform& GetWorldTransform(){return world_;}
};