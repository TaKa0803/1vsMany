#pragma once
#include"GlobalVariable/Tree/GlobalVariableTree.h"

//汎用マネージャの基底クラス
class BaseManager {

public:
	BaseManager()=default;
	~BaseManager()=default;

	/// <summary>
	/// デバッグツリー取得
	/// </summary>
	/// <returns>ツリーデータ</returns>
	GvariTree& GetTree() { return tree_; }

public:

	//デバッグ用パラメータツリー
	GvariTree tree_;

};