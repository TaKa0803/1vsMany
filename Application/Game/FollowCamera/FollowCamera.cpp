#include "FollowCamera.h"
#include"RandomNum/RandomNum.h"

FollowCamera::FollowCamera()
{
	camera_ = Camera::GetInstance();
	input_ = Input::GetInstance();
}

void FollowCamera::Update()
{
	//カメラ更新
	Vector2 stick;
	if (input_->IsControllerActive()) {
		stick = input_->GetjoyStickR();
	}
	else {
		Vector3 sti = input_->GetAllArrowKey();
		stick = { sti.x,sti.z };
	}
	stick.Normalize();
	stick.x *= xrotateNum;
	stick.y *= yrotatenum * -1.0f;
	camera_->AddCameraR_X(stick.y);
	camera_->AddCameraR_Y(stick.x);


	if (isShake_) {

		Vector3 pos = {
		RandomNumber::Get(-0.5f,0.5f),
		RandomNumber::Get(-0.5f,0.5f),
		0
		};

		Vector3 newp = tempP_ + pos;

		camera_->SetMainCameraPos(newp);

		if (cameraShakeCount_-- <= 0) {
			isShake_ = false;
			cameraShakeCount_ = 0;
			camera_->SetMainCameraPos(tempP_);
		}

	}

}

void FollowCamera::SetShake()
{
	//揺れていなかった場合
	if (!isShake_) {
		//一時座標を設定
		tempP_ = { 0,0,0 };
		//シェイクフラグをＯＮ
		isShake_ = true;
	}

	//シェイク時間をセット
	cameraShakeCount_ = count_;
}
