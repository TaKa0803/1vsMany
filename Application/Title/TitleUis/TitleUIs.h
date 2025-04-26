#pragma once
#include"Sprite/Sprite.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

enum class TitleSelect2Input
{
	Start,
	Leave,
	Count
};

//タイトルのUIまとめクラス
class TitleUIs {

public://**パブリック関数**//
	//コンストラクタ
	TitleUIs(TitleSelect2Input&select);
	//デストラクタ
	~TitleUIs() = default;
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

	/// <summary>
	/// デバッグツリー取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

private://**プライベート変数**//


	const TitleSelect2Input* select_;
	
	std::unique_ptr<Sprite> titleText_;

	std::unique_ptr<Sprite>startIcon_;

	std::unique_ptr<Sprite>dengenIcon_;

	std::unique_ptr<Sprite>windowTab_;

	std::unique_ptr<Sprite>selectWindow_;

	std::unique_ptr<Sprite>text_hajimeru_;
	std::unique_ptr<Sprite>text_owaru_;


	//点滅時間
	float tenmetuSec_ = 0;

	float maxTermetuSec_ = 1.0f;

	//逆算フラグ
	bool Inverse_ = false;

private://**デバッグ用**//

	//デバッグ用ツリー
	GvariTree tree_;


};