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

	Camera* camera_;

	Input* input_;

	Vector3 stRotate_{ 0,0,0 };

	//各角度での加算量
	float xrotateNum = 0.05f;
	float yrotatenum = 0.02f;

	bool isShake_ = false;
	int cameraShakeCount_ = 0;
	Vector2 shakenum = { 0,0 };
	Vector3 tempP_;

	int count_ = 3;
};