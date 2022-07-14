#include"Box.h"


Box::Box(XMFLOAT3 pos, uint16_t texture,XMFLOAT3 move,float M) {
	//ファイルを検索してテクスチャを読み込む
	textureHandle_ = texture;
	// 3Dモデルの生成
	model_ = Model::Create();

	worldtransform_.translation_.x = pos.x;
	worldtransform_.translation_.y = pos.y;
	worldtransform_.translation_.z = pos.z;

	affine.move.x = move.x / 50;
	affine.move.y = move.y / 50;
	affine.move.z = move.z / 50;

	m = M;

	//ワールドトランスフォームの初期化
	worldtransform_.Initialize();
	//ビュープロジェクションの初期化
	viewprojection_.Initialize();

}

Box::~Box() {
	delete model_;
}

void Box::Move() {
	worldtransform_.rotation_.x += affine.rota.x;
	worldtransform_.rotation_.y += affine.rota.y;
	worldtransform_.rotation_.z += affine.rota.z;

	worldtransform_.translation_.x += affine.move.x;
	worldtransform_.translation_.y += affine.move.y;
	worldtransform_.translation_.z += affine.move.z;

	worldtransform_.UpdateMatrix();
}

void Box::Draw() {
	model_->Draw(worldtransform_,viewprojection_, textureHandle_);
}