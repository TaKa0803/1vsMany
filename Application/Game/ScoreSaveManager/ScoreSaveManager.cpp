#include "ScoreSaveManager.h"
#include<cassert>
#include<json.hpp>
#include<fstream>
#include <iostream>
#include<Windows.h>

ScoreSaveManager::ScoreSaveManager()
{
}

void ScoreSaveManager::SaveScore(int killNum, int maxCombo)
{
	//データをセット
	scoreData_.kill = killNum;
	scoreData_.combo = maxCombo;

	nlohmann::json root;
	root = nlohmann::json::object();

	//jsonオブジェスト登録
	root[groupName_] = nlohmann::json::object();

	//値の登録
	root[groupName_][parameterName_[KIlls]] = scoreData_.kill;
	root[groupName_][parameterName_[Combos]] = scoreData_.combo;
	root[groupName_][parameterName_[HighKills]] = scoreData_.highKill;
	root[groupName_][parameterName_[HighCombos]] = scoreData_.highCombo;


	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists("Resources/jsonfile")) {
		std::filesystem::create_directory("Resources/jsonfile");
	}

	//書き込むJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName_ + ".json";
	//書き込み用ファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込み用に開く
	ofs.open(filepath);

	//ファイルオープン失敗?
	if (ofs.fail()) {
		std::string message = "Failed open file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	//ファイルにjson文字列を書き込む
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}

void ScoreSaveManager::LoadGroupData(const std::string& groupName)
{
	//よみこむJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName + ".json";
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に聞く
	ifs.open(filepath);

	// ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	nlohmann::json root;
	//json文字列からjsonのデータの構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();

	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	//未登録チェック
	assert(itGroup != root.end());


	//各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		//アイテム名を取得
		const std::string& itemName = itItem.key();

		if (itemName == parameterName_[KIlls]) {
			scoreData_.kill = itItem->get<int32_t>();
		}
		else if (itemName == parameterName_[Combos]) {
			scoreData_.combo = itItem->get<int32_t>();
		}
		else if (itemName == parameterName_[HighKills]) {
			scoreData_.highKill = itItem->get<int32_t>();
		}
		else if (itemName == parameterName_[HighCombos]) {
			scoreData_.highCombo = itItem->get<int32_t>();
		}
	}

}


void ScoreSaveManager::LoadScore()
{
	// ディレクトリがなければ返す
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		return;
	}

	std::filesystem::directory_iterator dir_it("Resources/jsonfile");
	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filepath = entry.path();

		//ファイル拡張子を取得
		std::string extention = filepath.extension().string();
		//.jsonファイル以外はスキップ
		if (extention.compare(".json") != 0) {
			continue;
		}

		if (filepath.stem().string() != groupName_) {
			continue;
		}

		LoadGroupData(filepath.stem().string());

	}
}
