#pragma once
#include"GlobalVariable/Tree/GlobalVariableTree.h"
#include<string>


//プレイヤーの音関係のまとめ
class PlayerSoundManager {

public://**パブリック変数**//
	enum AudioType {
		Punch,
		Kick,
		Drill,

		CountSound
	};
public://**パブリック関数**//

	PlayerSoundManager();
	~PlayerSoundManager() = default;

	/// <summary>
	/// 音の再生
	/// </summary>
	/// <param name="type">音の種類</param>
	void PlayAudio(AudioType type);

	/// <summary>
	/// 音の停止
	/// </summary>
	/// <param name="type">音の種類</param>
	void StopAudio(AudioType type);

	/// <summary>
	/// デバッグツリー取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

private:

	//音の配列番号
	int soundNumbers_[(size_t)CountSound];

	//音のボリューム
	float volumes_[(size_t)CountSound] = { 1.0f };

	//音のループフラグ
	bool isLoop_[(size_t)CountSound] = { false };

private:

	//デバッグ用パラメータツリー
	GlobalVariableTree tree_;

};