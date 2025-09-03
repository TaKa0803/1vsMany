#include"IScene/IScene.h"

int IScene::sceneNo = (int)SCENE::Debug;

bool IScene::leaveGame = false;

IScene::~IScene() {}

int& IScene::GetSceneNo() { return sceneNo; }

void IScene::SetScene(SCENE num) { sceneNo = (int)num; }
