#pragma once
#include"GlobalVariable/Tree/GlobalVariableTree.h"

//プレイヤーのボタン操作管理規定クラス
class PlayerBaseButtonManager {

public://**パブリック変数**//

	//デバッグツリー
	GvariTree tree_;

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerBaseButtonManager()=default;
	virtual ~PlayerBaseButtonManager()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init()=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()=0;

};