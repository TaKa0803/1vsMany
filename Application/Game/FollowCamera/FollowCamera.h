#pragma once
#include"Camera/Camera.h"
#include"Input/Input.h"

class FollowCamera {
public:

	FollowCamera();
	~FollowCamera()=default;

	void Init();

	void Update();

	void SetShake();

private:

	/// <summary>
	/// カメラ操作入力取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetInput();

	/// <summary>
	/// カメラシェイク更新
	/// </summary>
	void UpdateShake();
private:

	Camera* camera_;

	Input* input_;

	//カメラの最初の回転量
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