#pragma once
#include"IScene/IScene.h"

//クリアシーン
class ClearScene : public IScene {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClearScene();
	~ClearScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

};