#pragma once
#include "SingleGameObjects/GameObject.h"

class Plane :public GameObject {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Plane();
	~Plane()=default;

	//更新
	void Draw();


};

