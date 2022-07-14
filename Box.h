#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "Struct.h"

class Box {
  public:
	Box(XMFLOAT3 pos, uint16_t texture,XMFLOAT3 move,float M);

	~Box();

	void Move();

	void Draw();

private:
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;

  public:
	//ワールドトランスフォーム
	WorldTransform worldtransform_;
	//ビュープロジェクション
	ViewProjection viewprojection_;

	float m;

	Affine affine;
};