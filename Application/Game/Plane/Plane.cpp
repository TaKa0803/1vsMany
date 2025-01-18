#include "Plane.h"
#include<imgui.h>

#include "GlobalVariable/Group/GlobalVariableGroup.h"

Plane::Plane() {
	GameObject::Initialize("plane");


	GVariGroup* group =new GVariGroup("地面");
	group->SetTreeData(world_.GetDebugTree());
}

Plane::~Plane()
{
	
}

void Plane::Initialize() {
	
	world_.scale_ = { 500,500,500 };

	world_.UpdateMatrix();
}



void Plane::Draw() {
	world_.UpdateMatrix();
	GameObject::Draw();
}
