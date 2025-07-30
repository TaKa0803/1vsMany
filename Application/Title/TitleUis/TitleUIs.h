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
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBack();

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// デバッグツリー取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

private://**プライベート関数**//

	/// <summary>
	/// 選択物の点滅処理
	/// </summary>
	void BlinkingUpdate();

	/// <summary>
	/// 選択箇所のスプライト更新
	/// </summary>
	void SelectPositionUpdate();

private://**プライベート変数**//

	//入力のポインタ
	const TitleSelect2Input* select_;
	
	//タイトル背景
	std::unique_ptr<Sprite> titleText_;

	//スタートのアイコン
	std::unique_ptr<Sprite>startIcon_;

	//終了のアイコン
	std::unique_ptr<Sprite>dengenIcon_;

	//ウィンドウスプライト
	std::unique_ptr<Sprite>windowTab_;

	//選択スプライト
	std::unique_ptr<Sprite>selectWindow_;

	//はじめる文字スプライト
	std::unique_ptr<Sprite>text_hajimeru_;
	//おわる文字スプライト
	std::unique_ptr<Sprite>text_owaru_;


	//点滅時間
	float blinkingSec_ = 0;

	//点滅時間の最大時間
	float maxBlinkingSec_ = 1.0f;

	//逆算フラグ
	bool Inverse_ = false;

private://**デバッグ用**//

	//デバッグ用ツリー
	GvariTree tree_;
};