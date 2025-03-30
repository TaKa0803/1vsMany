#pragma once
#include"Sprite/Sprite.h"
#include<memory>
#include<string>


//遷移クラス
class Transition {
public:

	//遷移タイプ
	enum class TransitionType {
		Black2Clear,		//黒から透明へ
		Clear2Black,		//透明から黒へ
		CountTransitionType	//遷移の数
	}type_;

public:

	Transition();
	~Transition()=default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>遷移終了か</returns>
	bool Update();

	//描画
	void Draw();

	/// <summary>
	/// タイプを指定して遷移処理開始
	/// </summary>
	/// <param name="type"></param>
	void SetAndStartTransition(TransitionType type);

private:

	//状態ごとの更新テーブル
	static void (Transition::* TypeUpdate[])();

	//黒->透明の更新
	void Black2ClearUpdate();

	//透明-.黒の更新
	void Clear2BlackUpdate();

private:
	//シーンチェンジ用
	std::unique_ptr<Sprite>transitionSprite_;

	//シーンが変わりきる時間
	const float maxSceneChangeSec_ = 1.0f;

	//経過時間カウント
	float currentSceneXhangeSec_ = 0;

	//デバッグ用ツリー
	GvariTree tree_;
};