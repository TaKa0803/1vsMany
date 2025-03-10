#include"PlayerSoundManager.h"
#include"AudioManager/AudioManager.h"

PlayerSoundManager::PlayerSoundManager()
{
	//音の配列番号をタグから取得
	for (int i = 0; i < (int)CountSound; i++) {
		//読み込み済の配列番号取得
		soundNumbers_[i] = AudioManager::LoadSoundNum(pathTags_[i]);
	}

}

void PlayerSoundManager::PlayAudio(AudioType type)
{
	//size_tに修正
	size_t num = (size_t)type;
	//音の再生
	AudioManager::PlaySoundData(soundNumbers_[num], volumes_[num], isLoop[num]);
}

void PlayerSoundManager::StopAudio(AudioType type)
{
	//音の停止
	AudioManager::StopSound(soundNumbers_[(size_t)type]);
}
