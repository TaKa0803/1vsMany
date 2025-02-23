#pragma once
#include"Sprite/Sprite.h"
#include"Math/Vector3.h"
#include<algorithm>

//プレイヤー関連のUI管理
class PlayerUI {
public:

	PlayerUI(const int&comboCount);
	~PlayerUI()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private://**プライベート変数**//


	//ベーススキルアイコン
	std::unique_ptr<Sprite>baseSkillSp_;

	//次の技アイコン
	std::unique_ptr<Sprite>preSkillSprite_;

	//技アイコン
	std::unique_ptr<Sprite>skillSp_;

	//Bボタンアイコン
	std::unique_ptr<Sprite>BButton_;

	
	enum ButtonState {
		Wait,
		Punch,
		Kick,
		Ult
	};

	ButtonState BState_ = Wait;

private://**パラメータ**//

	const int* comboCount_=0;

	int skillTex_;
	int punchTex_;
	int kickTex_;
	int guruguruTex_;

	Vector3 waitATKpos = { 1000,622,0 };
	Vector3 waitATKscale = { 90,90 };

	Vector3 ATKpos = { 1085,560 };
	Vector3 ATKscale = { 200,200 };

};