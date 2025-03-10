#include"WorldTransform.h"
#include"Math/Matrix.h"
#include<imgui.h>

void EulerWorldTransform::Initialize() {
	//すべての値を初期値に
	translate_ = { 0.0f,0.0f,0.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	scale_ = { 1.0f,1.0f,1.0f };
	parent_ = nullptr;

	UpdateMatrix();
}

Matrix4x4 EulerWorldTransform::UpdateMatrix() {
	//行列を更新
	matWorld_ = MakeAffineMatrix(scale_, rotate_, translate_);
	//親がいる場合親の行列を含める
	if (parent_) {
		matWorld_ = matWorld_* parent_->matWorld_;
	}

	return matWorld_;
}



GlobalVariableTree& EulerWorldTransform::GetDebugTree(const std::string& name)
{
	//デバッグ用にパラメータをツリーに入れる
	tree_.name_=name;

	tree_.SetValue("座標", &translate_);
	tree_.SetValue("回転", &rotate_);
	tree_.SetValue("スケール", &scale_);

	return tree_;
}

GlobalVariableTree& EulerWorldTransform::GetDebugMonitorTree(const std::string& name)
{
	//モニターするパラメータをツリーに追加
	tree_.name_=name;

	tree_.SetMonitorValue("座標", &translate_);
	tree_.SetMonitorValue("回転", &rotate_);
	tree_.SetMonitorValue("スケール", &scale_);

	return tree_;
}







void QuaterinionWorldTransform::Initialize()
{
}

Matrix4x4 QuaterinionWorldTransform::UpdateMatrix()
{
	matWorld_ = MakeAffineMatrix(scale_, rotate_, translate_);

	if (parent_) {
		matWorld_ = matWorld_ * parent_->matWorld_;
	}

	return matWorld_;
}

//void QuaterinionWorldTransform::DrawDebug(const char* name)
//{
//#ifdef _DEBUG
//	if (ImGui::BeginMenu(name)) {
//		ImGui::DragFloat3("座標", &translate_.x, 0.01f);
//		ImGui::DragFloat3("回転", &rotate_.x, 0.01f);
//		ImGui::DragFloat3("拡縮", &scale_.x, 0.01f);
//		ImGui::EndMenu();
//	}
//#endif // _DEBUG
//}
