#pragma once
#include<string>

struct ScoreData {
	int kill=100;
	int combo=10;
	
	int highKill;
	int highCombo;
};

class ScoreSaveManager {

public:

	ScoreSaveManager();
	~ScoreSaveManager() = default;

	/// <summary>
	/// データをセーブする
	/// </summary>
	/// <param name="killNum">キル数</param>
	/// <param name="maxCombo">最大コンボ数</param>
	void SaveScore(int killNum, int maxCombo);

	/// <summary>
	/// スコアの読み込み
	/// </summary>
	void LoadScore();

	/// <summary>
	/// パラメータ取得
	/// </summary>
	/// <returns>パラメータ群</returns>
	const ScoreData& GetData() const { return scoreData_; };

private:

	void LoadGroupData(const std::string& groupName);

	enum ParameterName {
		KIlls,
		Combos,
		HighKills,
		HighCombos,
		CountParameterName
	};

	//各パラメータ名
	std::string parameterName_[CountParameterName] = {
		"kills",
		"combos",
		"highKills",
		"highCombos"
	};

	//パラメータデータ
	ScoreData scoreData_;

	std::string groupName_ = "SaveScore";

	//保存先ファイルパス
	const std::string kDirectoryPath = "Resources/jsonfile/";
};