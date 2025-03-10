#include "FollowCamera.h"
#include"RandomNum/RandomNum.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"DeltaTimer/DeltaTimer.h"

FollowCamera::FollowCamera()
{
	camera_ = Camera::GetInstance();
	input_ = Input::GetInstance();

	//デバッグ用にパラメータ設定
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("Camera");
	gvg->SetTreeData(camera_->mainCamera_.GetDebugTree("メインカメラ座標"));

	gvg->SetValue("加算量X", &xRotateNum);
	gvg->SetValue("加算量Y", &yRotateNum);

	gvg->SetValue("描画距離", &camera_->FarZ);
	gvg->SetValue("最小角度X", &camera_->minRotateX);
	gvg->SetValue("最大角度X", &camera_->maxRotateX);

	gvg->SetValue("カメラの初期角度", &stRotate_);
	gvg->SetValue("追従点との距離", &camera_->farFeaturedPos_);
	gvg->SetValue("座標のみの追従を行うか", &camera_->isOnlyGetPosition);

	gvg->SetValue("当たり判定処理フラグ", &camera_->isCollision_);
	gvg->SetValue("あった時に追加で寄せる量", &camera_->direction);

	//シェイクのパラメータをツリーに設定
	GvariTree tree;
	tree.name_ = "カメラシェイク";
	tree.SetMonitorValue("元の座標", &tempP_);	
	tree.SetValue("最大振動時間",&maxSecond_);
	tree.SetValue("振動幅", &shakeWide_);

	//ツリーをセット
	gvg->SetTreeData(tree);
}

void FollowCamera::Init()
{

	camera_->SetFocusPointRotate(stRotate_);
}

void FollowCamera::Update()
{
	//カメラ更新

	//入力を受け取る
	Vector2 stick=GetInput();

	//移動量分乗算
	stick.x *= xRotateNum;
	stick.y *= yRotateNum * -1.0f;
	//結果をカメラ本体に渡す
	camera_->AddCameraR_X(stick.y);
	camera_->AddCameraR_Y(stick.x);

	//カメラシェイクの更新
	UpdateShake();
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
	currentShakeSec_ = maxSecond_;
}

Vector2 FollowCamera::GetInput()
{
	//入力を保存する変数
	Vector2 stick;
	
	//コントローラ入力を取得
	stick = input_->GetjoyStickR();

	//キー入力を取得
	Vector3 sti = input_->GetAllArrowKey();
	stick += { sti.x, sti.z };

	//正規化して返却
	return stick.Normalize();
}

void FollowCamera::UpdateShake()
{
	//カメラシェイクのフラグが有効の場合
	if (isShake_) {
		Vector3 shakePos = {
		RandomNumber::Get(-shakeWide_,shakeWide_),
		RandomNumber::Get(-shakeWide_,shakeWide_),
		0
		};

		//ずれた値と主の値を加算した値を求める
		Vector3 newPosition = tempP_ + shakePos;

		//求めた値をセットする
		camera_->SetMainCameraPos(newPosition);

		//シェイク時間を減産処理
		currentShakeSec_ -= (float)DeltaTimer::deltaTime_;

		//時間が0以下の場合
		if (currentShakeSec_ <= 0) {
			//フラグをOFF
			isShake_ = false;
			//カメラ座標を元に戻す
			camera_->SetMainCameraPos(tempP_);
		}

	}
}
