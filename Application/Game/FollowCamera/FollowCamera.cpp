#include "FollowCamera.h"
#include"RandomNum/RandomNum.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

FollowCamera::FollowCamera()
{
	camera_ = Camera::GetInstance();
	input_ = Input::GetInstance();

	//デバッグ用にパラメータ設定
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("Camera");
	gvg->SetTreeData(camera_->mainCamera_.GetDebugTree("メインカメラ座標"));

	gvg->SetValue("描画距離", &camera_->FarZ);
	gvg->SetValue("最小角度X", &camera_->minRotateX);
	gvg->SetValue("最大角度X", &camera_->maxRotateX);

	gvg->SetValue("カメラの初期角度", &stRotate_);
	gvg->SetValue("追従点との距離", &camera_->farFeaturedPos_);
	gvg->SetValue("座標のみの追従を行うか", &camera_->isOnlyGetPosition);

	gvg->SetValue("当たり判定処理フラグ", &camera_->isCollision_);
	gvg->SetValue("あった時に追加で寄せる量", &camera_->direction);
}

void FollowCamera::Init()
{
	camera_->SetFocusPointRotate(stRotate_);
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
