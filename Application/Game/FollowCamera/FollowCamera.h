#pragma once
#include"Camera/Camera.h"
#include"Input/Input.h"

//追従カメラ
class FollowCamera {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FollowCamera(const EulerWorldTransform*target);
	~FollowCamera()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラを揺らす
	/// </summary>
	void SetShake();

private://**プライベート関数**//

	/// <summary>
	/// カメラ操作入力取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetInput();

	/// <summary>
	/// カメラシェイク更新
	/// </summary>
	void UpdateShake();

private://**プライベート変数**//

	//カメラのポインタ
	Camera* camera_;

	//入力のポインタ
	Input* input_;

	//カメラの最初の傾き
	Vector3 stRotate_{ 0,0,0 };

	//各角度での加算量
	float xRotateNum = 0.05f;
	float yRotateNum = 0.02f;

	//シェイクフラグ
	bool isShake_ = false;
	
	//振動時間
	float currentShakeSec_ = 0;

	//カメラの元の座標
	Vector3 tempP_;

	//最大振動時間
	float maxSecond_ = 3.0f;

	//振動幅
	float shakeWide_=0.5f;
};