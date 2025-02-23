#include "ScoreSaveManager.h"

ScoreSaveManager::ScoreSaveManager()
{
}

void ScoreSaveManager::SaveScore(int killNum, int maxCombo)
{
	//データをセット
	scoreData_.kill = killNum;
	scoreData_.combo = maxCombo;
}

void ScoreSaveManager::LoadScore()
{
}
