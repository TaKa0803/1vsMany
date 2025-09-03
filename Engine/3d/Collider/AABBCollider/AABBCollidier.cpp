#include"AABBCollidier.h"
#include<algorithm>


bool AABBCollider::isDraw_ = true;

void AABBCollider::Initialize(const std::string& tag, const EulerWorldTransform& parent) {
	//インスタンシングの初期化
	InstancingGameObject::Initialize("FallEnemy");
	//親子関係設定
	parent_ = (&parent);
	//コライダーのタグ設定
	colliderTag_ = tag;

	//画像を切る
	IMM_->SetEnableTexture(tag_, false);
	//ワイヤーフレーム表示
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	//影の削除
	IMM_->SetEnableShader(tag_, false);
	//透明度設定
	IMM_->SetAlpha(tag_, alpha_);

	localAabb_.maxV = { 0.5f,0.5f ,0.5f };
	localAabb_.minV = { -0.5f,-0.5f ,-0.5f };
}

void AABBCollider::Initialize(const std::string& tag) {
	//院スタン寝具の初期化
	InstancingGameObject::Initialize("box");

	//コライダーのタグ設定
	colliderTag_ = tag;

	//画像を切る
	IMM_->SetEnableTexture(tag_, false);
	//ワイヤーフレーム表示
	//IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	//影の削除
	IMM_->SetEnableShader(tag_, false);
	//透明度設定
	IMM_->SetAlpha(tag_, alpha_);

	localAabb_.maxV = { 0.5f,0.5f ,0.5f };
	localAabb_.minV = { -0.5f,-0.5f ,-0.5f };

	//ツリーに設定
	tree_.SetValue("scale", &scale_);

}

void AABBCollider::Update() {
	//過去の位置を保存
	preWorld_ = world_;

	SetAABBScale(scale_);

	//ワールドのスケールを見た目に合わせる
	world_.scale_ = localAabb_.maxV;

	//ワールド座標をターゲットに変更
	if (parent_) {
		world_.translate_ = parent_->GetWorldTranslate();
	}



	//座標を合わせたaabb生成
	aabb_ = {
		world_.translate_ + localAabb_.maxV,
		world_.translate_ + localAabb_.minV
	};

	CheckswitchAABB(aabb_);

	//ワールドデータを更新
	world_.UpdateMatrix();
}

void AABBCollider::UpdateMatrix() {
	//ワールド座標をターゲットに変更
	if (parent_) {
		world_.translate_ = parent_->GetWorldTranslate();
	}
	//座標を合わせたaabb生成
	aabb_ = {
		world_.translate_ + localAabb_.maxV,
		world_.translate_ + localAabb_.minV
	};

	CheckswitchAABB(aabb_);

	//ワールドデータを更新
	world_.UpdateMatrix();
}

void AABBCollider::Draw() {
	//デバッグ時のみ描画
#ifdef _DEBUG
	//描画フラグ有効時描画
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetData(tag_, world_, color_);
	}
#endif // _DEBUG
}

bool AABBCollider::isCollision(AABBCollider* aabbCollider, Vector3& backVec) {

	backVec = { 0, 0, 0 };

	if (!InCollision(aabb_, aabbCollider->aabb_)) {
		SetColor(false);
		aabbCollider->SetColor(false);
		return false;
	}

	SetColor(true);
	aabbCollider->SetColor(true);

	const AABB& a = aabb_;
	const AABB& b = aabbCollider->aabb_;

	// --- 各軸での重なり量 ---
	float overlapX = (std::min)(a.maxV.x, b.maxV.x) - (std::max)(a.minV.x, b.minV.x);
	float overlapY = (std::min)(a.maxV.y, b.maxV.y) - (std::max)(a.minV.y, b.minV.y);
	float overlapZ = (std::min)(a.maxV.z, b.maxV.z) - (std::max)(a.minV.z, b.minV.z);

	// --- 移動ベクトル（現在位置 - 過去位置） ---
	Vector3 velo = world_.GetWorldTranslate() - preWorld_.GetWorldTranslate();

	// --- velo がゼロ近い場合は中心差で方向を決定 ---
	const float epsilon = 1e-6f;
	if (fabs(velo.x) < epsilon && fabs(velo.y) < epsilon && fabs(velo.z) < epsilon) {
		Vector3 myCenter = world_.GetWorldTranslate();
		Vector3 otherCenter = aabbCollider->world_.GetWorldTranslate();
		velo = myCenter - otherCenter;
	}

	// --- 最小オーバーラップ軸を選択 ---
	float minOverlap = overlapX;
	char axis = 'x';

	if (overlapY < minOverlap) {
		minOverlap = overlapY;
		axis = 'y';
	}
	if (overlapZ < minOverlap) {
		minOverlap = overlapZ;
		axis = 'z';
	}

	// --- 押し戻し方向の決定 ---
	switch (axis) {
	case 'x':
		backVec.x = (velo.x >= 0) ? overlapX : -overlapX;
		break;
	case 'y':
		backVec.y = (velo.y >= 0) ? overlapY : -overlapY;
		break;
	case 'z':
		backVec.z = (velo.z >= 0) ? overlapZ : -overlapZ;
		break;
	}

	// --- 過去位置との整合性チェック ---
	// 押し戻し方向が「過去位置から見て逆方向」なら補正
	Vector3 pastToOther = preWorld_.GetWorldTranslate() - aabbCollider->world_.GetWorldTranslate();

	if (axis == 'x' && (pastToOther.x * backVec.x < 0)) {
		backVec.x = -backVec.x;
	}
	else if (axis == 'y' && (pastToOther.y * backVec.y < 0)) {
		backVec.y = -backVec.y;
	}
	else if (axis == 'z' && (pastToOther.z * backVec.z < 0)) {
		backVec.z = -backVec.z;
	}

	return true;
}

bool AABBCollider::IsCollision(const Segment& seg) {
	return InCollision(localAabb_, seg);
}

void AABBCollider::SetColor(bool hit) {
	//当たった時
	if (hit) {
		//当たった時の色
		color_ = hitColor;
	}
	else {
		//当たっていないときの色
		color_ = normalColor;
	}
}

void AABBCollider::SetAABBScale(const Vector3& scale) {
	//値の半値を最小最大に設定
	Vector3 s = {
		scale.x / 2,
		scale.y / 2,
		scale.z / 2
	};
	localAabb_.maxV = s;
	localAabb_.minV = s*-1.0f;
}