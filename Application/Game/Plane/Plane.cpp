#include "Plane.h"
#include "GlobalVariable/Group/GlobalVariableGroup.h"

Plane::Plane() {
	GameObject::Initialize("plane");

	//デバッグ値追加
	std::unique_ptr<GVariGroup> group =std::make_unique<GVariGroup>("plane");
	group->SetTreeData(model_->SetDebugParam());
	group->SetTreeData(world_.GetDebugTree());
}

void Plane::Draw() {
	world_.UpdateMatrix();
	GameObject::Draw();
}
