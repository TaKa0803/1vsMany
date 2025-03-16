#include"PlayerSoundManager.h"
#include"AudioManager/AudioManager.h"

PlayerSoundManager::PlayerSoundManager()
{
	//タグ名群
	std::string pathTags[(size_t)CountSound] = {
		"com1",
		"com2",
		"com3"
	};

	//音の配列番号をタグから取得
	for (int i = 0; i < (int)CountSound; i++) {
		//読み込み済の配列番号取得
		soundNumbers_[i] = AudioManager::LoadSoundNum(pathTags[i]);
	}

	//デバッグ用にパラメータ追加
	tree_.name_ = "音関係";

	for (int i = 0; i < CountSound; i++) {
		GvariTree newTree;
		newTree.name_ = pathTags[i];
		newTree.SetValue("音量", &volumes_[i]);
		newTree.SetValue("ループ", &isLoop_[i]);
		tree_.SetTreeData(newTree);
	}


}

void PlayerSoundManager::PlayAudio(AudioType type)
{
	//size_tに修正
	size_t num = (size_t)type;
	//音の再生
	AudioManager::PlaySoundData(soundNumbers_[num], volumes_[num], isLoop_[num]);
}

void PlayerSoundManager::StopAudio(AudioType type)
{
	//音の停止
	AudioManager::StopSound(soundNumbers_[(size_t)type]);
}
